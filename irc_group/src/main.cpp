/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:17:11 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/25 21:29:55 by amenses-         ###   ########.fr       */
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

/* TODO:
    - Socket creation, binding and listening
    - Client sockets handling
    - Parsing errors handling
    - Chaining reading
*/

// handle params and response
int main(int argc, char **argv)
{
    if (argc != 3 || !is_valid_data(argv))
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        std::cerr << "Example: " << argv[0] << " 4242 mysecretpassword" << std::endl;
        return EXIT_FAILURE;
    }
    uint16_t port = static_cast<uint16_t>(atol(argv[1]));
    std::string password = std::string(argv[2]);
    Server server(port, password);
    SignalHandler signal_handler(server);
    // signal_handler.config();
    server.create_socket(); //maybe do the listening socket set up altogether
    server.bind_socket();
    server.listen_socket();
    Response::set_server(&server);
    Logger::Log(INFO, "Hm...");
    // print server timeout members
    Logger::Log(INFO, "server running: " + to_string_(server.is_running()));

    while (server.is_running())
    {
        server.get_connection();
        server.process_message();
        server.check_connection();
        server.delete_broken_connection();
        server.delete_empty_channels();
        Response::reset();
        // Logger::Log(DEBUG, "Server is running...");
        // server.ping_users();
    }
    return EXIT_SUCCESS;
}
