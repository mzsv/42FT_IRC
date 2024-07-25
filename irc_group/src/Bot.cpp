/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:21 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/26 00:46:01 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(int port, std::string password) :
    port_(port), password_(password), nickname_("bot"),
    server_("0.0.0.0"), socket_fd_(-1)
{
}

Bot::~Bot()
{
}

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

void Bot::send_message(const std::string& message)
{
    if (message.size() > 0)
    {
        if (send(socket_fd_, message.c_str(), message.size(), 0) < 0)
        {
            Logger::Log(ERROR, "Bot::Failed to send message: " + message);
        }
        Logger::Log(INFO, "Bot::Sent message: " + message);
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
    Logger::Log(DEBUG, "Bot::Received message: " + msg);
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
        say_hello(channel);
    }
    else if (command == "PRIVMSG" || command == "NOTICE")
    {
        reply(message);
    }
}

void Bot::say_hello(const std::string& target)
{
    send_to(target, " :Hello! I'm da Bot in da place. Let's play a game of TicTacToe! Type 'play game' to start!\n");
}

void Bot::reply(const Message& message)
{
    std::string target = message.get_arguments()[0];
    std::string reply;
    if (target[0] != '#')
    {
        target = message.get_prefix().substr(0, message.get_prefix().find('!'));
    }
    if (message.get_trailing().find("play") == 0)
    {
        if (!game_)
        {
            game_ = new TicTacToe(this, target); // memory leak
        }
        else
        {
            game_->play_round(message.get_trailing());
        }
    }
    std::string response = "PRIVMSG " + target + " :" + reply + "\r\n";
    send_message(response);
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
            Logger::Log(DEBUG, "Bot::Handling message: " + message.get_message());
            Logger::Log(DEBUG, "Bot::Command: " + message.get_command());
            Logger::Log(DEBUG, "Bot::Prefix: " + message.get_prefix());
            Logger::Log(DEBUG, "Bot::Trailing: " + message.get_trailing());
            handle_message(message);
        }
    }
}

void Bot::stop()
{
    close(socket_fd_);
    socket_fd_ = -1;
    Logger::Log(INFO, "Bot::Disconnecting from server...");
}

void Bot::send_to(const std::string& target, const std::string& message)
{
    send_message("PRIVMSG " + target + " :" + message);
}
