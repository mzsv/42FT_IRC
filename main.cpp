/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:23:51 by amenses-          #+#    #+#             */
/*   Updated: 2024/05/22 22:25:38 by amenses-         ###   ########.fr       */
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

// addrinfo > socket > bind > listen > accept
int main()
{
    struct addrinfo hints, *res, *i;
    int             listener;
    int             activate = 1;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // use my IP
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
    {
        std::cout << "getaddrinfo error" << std::endl;
        return (1);
    }
    for (i = res; i != NULL; i = i->ai_next)
    {
        std::cout << "ai_protocol: " << i->ai_protocol << std::endl;
        if ((listener = socket(res->ai_family, res->ai_socktype, \
            res->ai_protocol)) == -1)
        {
            std::cout << "socket error" << std::endl;
            continue ;
        }
        fcntl(listener, F_SETFL, O_NONBLOCK); // non-blocking
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
    // print socket address
    struct sockaddr_in* addr_in = (struct sockaddr_in *) i->ai_addr;
    char*               ip = inet_ntoa(addr_in->sin_addr);

    printf("family: %d\n", addr_in->sin_family);
    std::cout << "server: listening on " << ip << ":" << PORT << std::endl;

    // poll
    int             poll_count = 0;
    int             nfds = 1;
    int             fds_size = 3;
    struct pollfd*  fds = (struct pollfd*)malloc(sizeof(struct pollfd) * fds_size);

    fds[0].fd = listener;
    fds[0].events = POLLIN;
    while (1)
    {
        // std::cout << "polling" << std::endl;
        // std::cout << "nfds: " << nfds << std::endl;
        // std::cout << "----------------------\n" << std::endl;
        if ((poll_count = poll(fds, nfds, -1)) == -1)
        {
            std::cout << "poll error" << std::endl;
            return (1);
        }
        // std::cout << "poll_count: " << poll_count << std::endl;
        // std::cout << "nfds: " << nfds << std::endl;
        // for (int i = 0; i < nfds; ++i) {
        //     std::cout << fds[i].fd << " " << std::endl;
        // }
        // check for events
        for (int i = 0; i < nfds; i++)
        {
            if (fds[i].revents & POLLIN) // maybe a separate handle for POLLIN and POLLOUT ?!
            {
                // std::cout << "POLLIN: " << i << std::endl;
                if (fds[i].fd == listener)
                {
                    // handle listener's incoming connection (new client)
                    struct sockaddr_storage client_addr;
                    socklen_t               client_addr_len = sizeof(client_addr);
                    int                     client_fd;

                    // accept
                    if ((client_fd = accept(listener, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len)) == -1)
                    {
                        std::cout << "accept error" << std::endl;
                        close(listener); // prevents further incoming connections
                        // return (1);
                    }
                    else
                    {
                        fcntl(client_fd, F_SETFL, O_NONBLOCK); // non-blocking
                        // close(listener); // limit to one connection for testing
                        char client_ip[INET6_ADDRSTRLEN];

                        inet_ntop(client_addr.ss_family, get_sockaddr_in(reinterpret_cast<struct sockaddr*>(&client_addr)), client_ip, sizeof(client_ip));
                        std::cout << "server: accepted connection from " << client_ip \
                            << " on socket " << client_fd << std::endl;
                        if (nfds >= fds_size)
                        {
                            fds_size *= 2;
                            fds = (struct pollfd*)realloc(fds, sizeof(struct pollfd) * fds_size);
                        }
                        fds[nfds].fd = client_fd;
                        fds[nfds].events = POLLIN | POLLOUT; // read and write
                        nfds++;
                    }
                    fds[i].revents = 0;
                    //join channel test
                    // std::string join = "JOIN #nc_channel\r\n";
                    // send(client_fd, join.c_str(), join.size(), 0);
                    //send PING
                    // std::string ping = "PING aaa\r\n";

                    // send(client_fd, ping.c_str(), ping.size(), 0);
                    
                }
                else
                // handle client's incoming message (not listener)
                {
                    // write
                    // if (fds[i].revents & POLLOUT) {
                    //     char* message = "Hello, client!\n";
                    //     send(client_fd, message, strlen(message), 0);
                    //     std::cout << "message sent" << std::endl;
                    // }
                    
                    std::cout << "reading" << std::endl;
                    
                    // read
                    int bytes_read;
                    char buffer[1024];
                    
                    bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    std::cout << "bytes_read: " << bytes_read << std::endl;
                    if (bytes_read == 0) {
                        std::cout << "client " << fds[i].fd << " disconnected" << std::endl;
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        continue ;
                    }
                    buffer[bytes_read] = 0;
                    std::cout << "bytes_read: " << bytes_read << std::endl;
                    std::cout << "buffer: " << buffer << std::endl;
                    // continue ;

                    // hello message
                    // std::string hello = ":server_name PRIVMSG nc_user :hello\r\n";
                    
                    //send to all clients
                    for (int j = 1; j < nfds; ++j) {
                        if (j != i && fds[j].fd > 0) {
                            send(fds[j].fd, buffer, bytes_read, 0);
                        }
                    }
                }
            }
        }
    }
    close(listener);
    return (0);
}
