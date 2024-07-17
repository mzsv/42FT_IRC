/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:17:11 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/17 17:00:00 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>

#include "Server.hpp"
#include "Logger.hpp"

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
    Server server = Server(port, password);
    server.create_socket(); //maybe do the listening socket set up altogether
    server.bind_socket();
    server.listen_socket();
    Logger::Log(INFO, "Hm...");
    // print server timeout members

    while (true)
    {
        server.get_connection();
        server.process_message();
        server.check_connection();
        server.delete_broken_connection();
        server.delete_empty_channels();
        // Logger::Log(DEBUG, "Server is running...");
        // server.ping_users();
    }

    return EXIT_SUCCESS;
}
