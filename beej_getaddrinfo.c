/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   beej_getaddrinfo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:03 by amenses-          #+#    #+#             */
/*   Updated: 2023/12/02 00:39:01 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
            printf("sin_addr: %s\n", inet_ntoa(ipv4->sin_addr));
            printf("sin_port: %d\n", ntohs(ipv4->sin_port));
            printf("sin_family: %d\n", ipv4->sin_family);
            printf("sin_zero: %s\n", ipv4->sin_zero);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
            // printf("sin_addr: %s\n", inet_ntoa(ipv6->sin6_addr)); // only IPv4
            printf("sin_port: %d\n", ntohs(ipv6->sin6_port));
            printf("sin_family: %d\n", ipv6->sin6_family);
            // printf("sin_zero: %s\n", ipv6->sin6_zero); // only IPv4
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
        //my tests
        // printf("ntoa: %s\n", inet_ntoa(((struct sockaddr_in *)p->ai_addr)->sin_addr)); // IPv4 only
        // printf("port: %d\n", ((struct sockaddr_in *)p->ai_addr)->sin_port);
        printf("socktype: %d, SOCK_STREAM: %d\n", p->ai_socktype, SOCK_STREAM);
        printf("protocol: %d, IPPROTO_TCP: %d\n", p->ai_protocol, IPPROTO_TCP);
        printf("addrlen: %d\n", p->ai_addrlen);
        printf("flags: %d\n", p->ai_flags);
        printf("family: %d, AF_INET: %d, AF_INET6: %d\n", p->ai_family, AF_INET, AF_INET6);
        printf("sock data: %s\n", (char *)p->ai_addr->sa_data); // nothing...
        printf("-------------------------\n"); // nothing...
        printf("AF_UNSPEC: %d\n", AF_UNSPEC);
        printf("AI_PASSIVE: %d\n", AI_PASSIVE);
    }

    freeaddrinfo(res); // free the linked list

    return 0;
}
