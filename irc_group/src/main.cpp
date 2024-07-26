/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:17:11 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/26 17:59:56 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>

#include "Server.hpp"
#include "Logger.hpp"
#include "SignalHandler.hpp"

bool is_valid_data(char **argv);

int main(int argc, char **argv)
{
    if (argc != 3 || !is_valid_data(argv))
    {
        Logger::Log(ERROR, "Usage: ./ircserv <port> <password>");
        Logger::Log(INFO, "Example: ./ircserv 4242 myservpassword");
        return EXIT_FAILURE;
    }
    uint16_t port = static_cast<uint16_t>(atol(argv[1]));
    std::string password = std::string(argv[2]);
    Server server(port, password);
    SignalHandler signal_handler(server);
    server.create_socket();
    server.bind_socket();
    server.listen_socket();
    Response::set_server(&server);

    while (server.is_running())
    {
        server.get_connection();
        server.process_message();
        server.check_connection();
        server.delete_broken_connection();
        server.delete_empty_channels();
        Response::reset();
    }
    return EXIT_SUCCESS;
}
