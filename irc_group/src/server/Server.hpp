/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:12 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/25 21:36:01 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <string>
#include <vector>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <ctime>
#include <iostream>

class User;
class Channel;
class Executor;

#include "User.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "Executor.hpp"

#include "irc_err_codes.hpp"
#include "irc_rpl_codes.hpp"

#define DISCONNECT -2

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif


class Server
{
  private:
	int port_;
	sockaddr_in sockaddr_;
	int socket_fd_;

	std::vector<User*> users_;
	std::vector<struct pollfd> users_fds_;
	std::map<std::string, Channel*> channels_;
	// std::map<Channel*, std::set<User*>> operators;
	std::string name_;
	std::string password_;

	// time_t max_inactive_time_;
	// time_t max_response_time_;
	const id_t timeout_; // used for poll(); shoudnt it be 0? check man
	double max_inactive_time_;
	double max_response_time_;

  public:
	Server(int port, const std::string& password);
	~Server();

	/**
	 * Getters
	*/
	int get_socket_fd() const;
	const std::string& get_name() const;
	const std::string& get_password() const;

	/**
	 * Setters
	*/
	//

	/**
	 * Funcs
	*/
	void create_socket();
	void bind_socket();
	void listen_socket();
    void delete_broken_connection();
    void delete_empty_channels();
    void check_connection();
	int check_connection(User& user);
    void get_connection();
    void process_message();
    int handle_message(User& user);
	void notify_users(User& user, const std::string& message);
	bool contains_nickname(const std::string& nickname) const;

	// void ping_users() const; // already at server::check_connection()

	int join_channel(const std::string& name, const std::string& key, const User& creator);
};


#endif // SERVER_SERVER_HPP
