/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:12 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/26 19:53:32 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "Logger.hpp"
#include "to_string_.hpp"
#include "ston.hpp"

#define DISCONNECT -2
#define NR_OF_SERVERS 1

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
	std::map<std::string,std::string> isupport_params_;
	std::vector<User*> users_;
	std::vector<struct pollfd> users_fds_;
	std::map<std::string, Channel*> channels_;
	std::vector<std::string> invited_users_;
	std::string name_;
	std::string password_;
	const id_t timeout_;
	double max_inactive_time_;
	double max_response_time_;
	time_t start_time_;
	std::string description;
	std::string version;
	std::string available_channel_modes;
	size_t max_local_users_;
	static bool running_;

	/**
     * Forbidden APIs
    */
   	Server();
	Server(const Server& obj);
	Server& operator=(const Server& obj);

  public:
	Server(int port, const std::string& password);
	~Server();

	/**
	 * Getters
	*/
	const std::map<std::string, std::string>& get_isupport_params() const;
	const std::string& get_isupport_param(const std::string& key) const;
	int get_socket_fd() const;
	const std::string& get_name() const;
	const std::string& get_password() const;
	const std::map<std::string, Channel*>& get_channels() const;
	User* get_user(const std::string& nickname) const;
	const time_t& get_start_time() const;
	const std::string& get_description() const;
	const std::string& get_version() const;
	const std::string& get_available_channel_modes() const;
	const std::vector<User*>& get_users() const;
	const size_t& get_max_local_users() const;

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
	const bool& is_running() const;
	int join_channel(const std::string& name, const std::string& key, User& creator);
	bool contains_channel(const std::string& name) const;
	bool user_on_channel(const std::string& channel, const User& user) const;
	bool user_on_channel(const std::string& channel, const std::string& user) const;
	bool is_operator(const std::string& channel, const User& user) const;
	void leave_channel(const std::string& name, User& user);
	void channel_broadcast(const std::string& channel_name, const User& user, const std::string& message) const; // !
	bool check_channel_mode(const std::string& channel_name, const unsigned char& mode) const;
	const std::string get_channel_topic(const std::string& channel_name) const;
	void stop();
};

#endif // SERVER_HPP
