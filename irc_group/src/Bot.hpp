/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:13 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 00:26:21 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

class Bot
{
    private:
        Bot();
        Bot(const Bot& obj);
        Bot& operator=(const Bot& obj);

        std::string nickname_;
        int port_;
        int socket_fd_;
        std::string channel_;
        std::string server_;
        sockaddr_in sockaddr_;
        

    public:
        Bot(const std::string& server, const std::string& channel, const std::string& nickname, int port);
        ~Bot();

        void connect_to_server();
        

        void say_hello();
};

#endif // BOT_HPP
