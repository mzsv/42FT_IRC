/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:21 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 00:32:38 by amenses-         ###   ########.fr       */
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

Bot::Bot(const std::string& server, const std::string& channel, const std::string& nickname, int port) :
    nickname_(nickname), port_(port), channel_(channel), server_(server)
{
    // struct sockaddr_in serv_addr;
    // struct hostent *server;

    // socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    // if (socket_fd_ < 0)
    // {
    //     std::cerr << "Error opening socket" << std::endl;
    //     exit(1);
    // }
    // server = gethostbyname(server.c_str());
    // if (server == NULL)
    // {
    //     std::cerr << "Error, no such host" << std::endl;
    //     exit(1);
    // }
    // bzero((char *) &serv_addr, sizeof(serv_addr));
    // serv_addr.sin_family = AF_INET;
    // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    // serv_addr.sin_port = htons(port_);
    // if (connect(socket_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    // {
    //     std::cerr << "Error connecting" << std::endl;
    //     exit(1);
    // }
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
