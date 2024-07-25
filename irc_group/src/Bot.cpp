/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:21 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 19:47:48 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"


template <typename T> // remove !
std::string to_string_(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

Bot::Bot(int port, std::string password) :
    port_(port), password_(password),
    server_("0.0.0.0"), socket_fd_(-1), nickname_("bot")
{
}

Bot::~Bot()
{
    close(socket_fd_);
}

void Bot::connect_to_server()
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(server_.c_str(), to_string_(port_).c_str(), &hints, &res) != 0)
    {
        std::cerr << "Failed to get address info." << std::endl;
        exit(1);
    }
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd_ < 0)
    {
        Logger::Log(ERROR, "Failed to create socket");
        socket_fd_ = -1;
        return ;
    }
    if (connect(socket_fd_, res->ai_addr, res->ai_addrlen) < 0)
    {
        Logger::Log(ERROR, "Failed to connect to server");
        socket_fd_ = -1;
        freeaddrinfo(res);
        return ;
    }
    freeaddrinfo(res);
    send_message("PASS " + password_);
    send_message("NICK " + nickname_);
    send_message("USER " + nickname_ + " 0 * :" + nickname_);
    // socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    // if (socket_fd_ < 0)
    // {
    //     std::cerr << "Failed to create socket." << std::endl;
    //     exit(1);
    // }
    // sockaddr_.sin_family = AF_INET;
    // sockaddr_.sin_addr.s_addr = inet_addr(server_.c_str());
    // sockaddr_.sin_port = htons(port_);
    // if (connect(socket_fd_, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_)) < 0)
    // {
    //     std::cerr << "Failed to connect to server." << std::endl;
    //     exit(1);
    // }
    
}

void Bot::send_message(const std::string& message)
{
    if (message.size() > 0)
    {
        if (send(socket_fd_, message.c_str(), message.size(), 0) < 0)
        {
            Logger::Log(ERROR, "Failed to send message: " + message);
        }
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
    if (bytes < 0)
    {
        Logger::Log(ERROR, "Failed to receive message");
        socket_fd_ = -1;
        return ;
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
        send_message("PONG " + message.get_trailing());
    }
    else if (command == "INVITE")
    {
        send_message("JOIN " + message.get_trailing());
    }
    else if (command == "PRIVMSG" || command == "NOTICE")
    {
        reply(message);
    }
}

void Bot::reply(const Message& message)
{
    std::string target = message.get_prefix();
    std::string command = message.get_command();
    std::string reply = "I'm a bot! Work it, work it, bam, work it...";
    std::string response = ":" + nickname_ + " " + command + " " + target + " :" + reply;
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
            handle_message(message);
        }
    }
}
