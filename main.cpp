/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:23:51 by amenses-          #+#    #+#             */
/*   Updated: 2023/11/18 19:36:14 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
