/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:23:51 by amenses-          #+#    #+#             */
/*   Updated: 2023/12/02 00:40:01 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// .ircserv <port> <password>
// agrc = 3
// port: listening port number
// multiple clients
// non-blocking I/O operations only
// choose one IRC client as reference
// communication server-client: TCP/IP (v4 or v6)
// Features:
// - authentication
// - set nickname
// - set username
// - join channel
// - send/receive private messages (using reference client)
// - send/receive channel messages (using reference client)
// - operators and regular users
// - channel operator commands:
//      .kick
//      .invite
//      .topic: change or view channel topic
//      .mode:
//          +i: set/remove channel invite-only flag
//          +t: set/remove restrictions on topic changing to chanOps only
//          +k: set/remove channel key (password)
//          +o: give/take channel operator privileges
//          +l: set/remove user limit to channel (number of users?)

// get sockadrr_in struct
void*   get_sockaddr_in(struct sockaddr *addr)
{
    if (addr->sa_family == AF_INET)
    {
        return (reinterpret_cast<struct sockaddr_in*>(addr));
    }
    return (reinterpret_cast<struct sockaddr_in6*>(addr));
}

// get sockfd
/* int     get_sockfd(struct addrinfo *addr_info)
{
    int sockfd = -1;

    // while != -1?
    // recursive
    for (struct addrinfo* i = addr_info; i != NULL; i = i->ai_next)
    {
        if ((sockfd = socket(addr_info->ai_family, addr_info->ai_socktype, \
            addr_info->ai_protocol)) == -1)
        {
            std::cout << "socket error" << std::endl;
            return (-1);
        }
        else
        {
            break ;
        }
    }
    return (sockfd);
} */

int main()
{
    struct addrinfo hints, *res, *i;
    int             listener;
    int             activate = 1;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // use my IP
    // addrinfo > socket > bind > listen > accept
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
    {
        std::cout << "getaddrinfo error" << std::endl;
        return (1);
    }
    // socket
    // if ((listener = get_sockfd(res)) == -1)
    // {
    //     return (1);
    // }
    for (i = res; i != NULL; i = i->ai_next)
    {
        if ((listener = socket(res->ai_family, res->ai_socktype, \
            res->ai_protocol)) == -1)
        {
            std::cout << "socket error" << std::endl;
            continue ;
        }
        // setsockopt
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(int)) == -1)
        {
            std::cout << "setsockopt error" << std::endl;
            return (1);
        }
        // bind
        if (bind(listener, res->ai_addr, res->ai_addrlen) == -1)
        {
            std::cout << "bind error" << std::endl;
            close(listener);
            return (1);
        }
        break ;
    }
    freeaddrinfo(res);
    if (i == NULL)
    {
        std::cout << "failed to bind" << std::endl;
        return (1);
    }
    // listen
    if (listen(listener, MAX_CLIENTS) == -1)
    {
        std::cout << "listen error" << std::endl;
        return (1);
    }
    // socket address
    // char ip[INET6_ADDRSTRLEN];
    // inet_ntop(i->ai_family, get_sockaddr_in(reinterpret_cast<struct sockaddr*>(&i->ai_addr)), ip, sizeof(ip));
    std::cout << "server: listening on " << ip << ":" << PORT << std::endl;
    // accept
    struct sockaddr_storage client_addr;
    socklen_t               client_addr_len = sizeof(client_addr);
    int                     client_fd;

    while (1)
    {
        if ((client_fd = accept(listener, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len)) == -1)
        {
            std::cout << "accept error" << std::endl;
            close(listener);
            return (1);
        }
        else
        {
            char client_ip[INET6_ADDRSTRLEN];
            inet_ntop(client_addr.ss_family, get_sockaddr_in(reinterpret_cast<struct sockaddr*>(&client_addr)), client_ip, sizeof(client_ip));
            std::cout << "server: got connection from " << client_ip << std::endl;
        }
    }
    return (0);
}
