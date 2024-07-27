/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:21 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/27 15:24:06 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "TicTacToe.hpp"

Bot::Bot(int port, std::string password) :
    port_(port), password_(password), nickname_("bot"),
    server_("0.0.0.0"), socket_fd_(-1), game_(NULL)
{
}

Bot::~Bot()
{
    if (game_)
    {
        delete game_;
    }
}

/**
 * Funcs
*/
void Bot::connect_to_server()
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(server_.c_str(), to_string_(port_).c_str(), &hints, &res) != 0)
    {
        return stop();
    }
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd_ < 0)
    {
        return stop();
    }
    if (connect(socket_fd_, res->ai_addr, res->ai_addrlen) < 0)
    {
        freeaddrinfo(res);
        return stop();
    }
    freeaddrinfo(res);
    send_message("PASS " + password_ + "\r\n");
    send_message("NICK " + nickname_ + "\r\n");
    send_message("USER " + nickname_ + " 0 * :" + nickname_ + "\r\n");
}

void Bot::send_message(const std::string& message) const
{
    if (message.size() > 0)
    {
        if (send(socket_fd_, message.c_str(), message.size(), 0) < 0)
        {
            Logger::Log(ERROR, "Failed to send message: " + message);
        }
        Logger::Log(INFO, "(Sent) " + message);
    }
}

void Bot::receive_message()
{
    char buffer[100];
    int bytes;
    std::string msg;
    
    if (messages_.size() > 0)
    {
        msg = messages_.front();
    }
    while ((bytes = recv(socket_fd_, buffer, 99, 0)) > 0)
    {
        buffer[bytes] = 0;
        msg += buffer;
        memset(buffer, 0, sizeof(buffer));
        if (msg.find("\r\n") != std::string::npos)
        {
            break ;
        }
    }
    if (bytes <= 0)
    {
        return stop();
    }
    while (msg.find("\r\n") != std::string::npos)
	{
		msg.replace(msg.find("\r\n"), 2, "\n");
	}
	if (msg.size() > 1)
	{
		messages_ = split2queue(msg, '\n', true);
	}
}

void Bot::handle_message(const Message& message)
{
    std::string command = message.get_command();
    if (command == "PING")
    {
        send_message("PONG " + message.get_trailing() + "\r\n");
    }
    else if (command == "INVITE")
    {
        std::string channel = message.get_trailing();
        send_message("JOIN " + channel + "\r\n");
    }
    else if (command == "JOIN")
    {
        say_hello(message.get_arguments()[0]);
    }
    else if (command == "PRIVMSG" || command == "NOTICE")
    {
        reply(message);
    }
    else if (command == "PART" || command == "QUIT")
    {
        send_to(message.get_arguments()[0], "One human less... This is getting cute!\n");
    }
}

void Bot::say_hello(const std::string& target)
{
    send_to(target, "Hello! I'm da Bot in da place. Let's play a game of TicTacToe? Type 'play game' to start!\n");
}

void Bot::reply(const Message& message)
{
    std::string target = message.get_arguments()[0];
    std::string text = message.get_trailing();
    if (target[0] != '#')
    {
        target = message.get_prefix().substr(0, message.get_prefix().find('!'));
    }
    if (text.find("play") == 0)
    {
        if (text.find("play game") == 0)
        {
            if (!game_)
            {
                Logger::Log(INFO, "Starting new game");
                game_ = new TicTacToe(this, target);
            }
            else
            {
                send_to(target, "Game already in progress... Hold your horses :)\n");
            }
        }
        else if (game_ && target == game_->get_target() && game_->play_round(text))
        {
            end_game();
        }
    }
    else if (text.find("stop game") == 0)
    {
        if (game_)
        {
            end_game();
        }
        else
        {
            send_to(target, "No game in progress... What are you talking about?\n");
        }
    }
    else if (text.find("bot") == 0)
    {
        send_to(target, "What's up? If you wanna play, you already know what to do. Otherwise, I'll be sunbathing, so don't bother me, thank you!\n");
    }
}

void Bot::run()
{
    connect_to_server();
    while (socket_fd_ > 0)
    {
        receive_message();
        while (messages_.size() > 0)
        {
            Message message(messages_.front());
            messages_.pop();
            Logger::Log(DEBUG, "Handling message: " + message.get_message());
            Logger::Log(DEBUG, "Command: " + message.get_command());
            Logger::Log(DEBUG, "Prefix: " + message.get_prefix());
            Logger::Log(DEBUG, "Trailing: " + message.get_trailing());
            handle_message(message);
        }
    }
}

void Bot::stop()
{
    close(socket_fd_);
    socket_fd_ = -1;
    Logger::Log(INFO, "Disconnecting from server...");
}

void Bot::send_to(const std::string& target, const std::string& message) const
{
    send_message("PRIVMSG " + target + " :" + message);
}

void Bot::end_game()
{
    if (game_)
    {
        send_to(game_->get_target(), "Game over! Type 'play game' to start a new one!\n");
        delete game_;
        game_ = NULL;
        Logger::Log(INFO, "Game ended");
    }
}
