/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   beej_poll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:54:45 by amenses-          #+#    #+#             */
/*   Updated: 2023/11/21 17:49:19 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main(void)
{
    struct pollfd pfds[1]; // More if you want to monitor more

    pfds[0].fd = 0;          // Standard input
    pfds[0].events = POLLIN; // Tell me when ready to read

    // If you needed to monitor other things, as well:
    //pfds[1].fd = some_socket; // Some socket descriptor
    //pfds[1].events = POLLIN;  // Tell me when ready to read

    while (1)
    {
        printf("Hit RETURN or wait 2.5 seconds for timeout\n");

        int num_events = poll(pfds, 1, 2500); // 2.5 second timeout

        if (num_events == 0) {
            printf("Poll timed out!\n");
        } else {
            int pollin_happened = pfds[0].revents & POLLIN;

            if (pollin_happened) {
                printf("File descriptor %d is ready to read\n", pfds[0].fd);
            } else {
                printf("Unexpected event occurred: %d\n", pfds[0].revents);
            }
        }
    }
    return 0;
}
