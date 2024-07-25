/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 23:14:13 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 19:47:36 by amenses-         ###   ########.fr       */
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
#include "Logger.hpp"
#include "Message.hpp"
#include "ston.hpp"

class Bot
{
    private:
        Bot();
        Bot(const Bot& obj);
        Bot& operator=(const Bot& obj);

        int port_;
        std::string password_;
        const std::string nickname_;
        std::string server_;
        int socket_fd_;
        sockaddr_in sockaddr_;
        std::queue<std::string> messages_;
        
        void connect_to_server();
        void send_message(const std::string& message);
        void receive_message();
        void handle_message(const Message& message);
        void reply(const Message& message);

        void say_hello();
        
    public:
        Bot(int port, std::string password);
        ~Bot();

        void run();
};

#endif // BOT_HPP
