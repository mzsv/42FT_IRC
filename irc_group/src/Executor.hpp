/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:21:04 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/13 20:18:45 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <sstream>
#include "Message.hpp"
#include "User.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "irc_err_codes.hpp"
// #include "irc_codes.hpp"

/**
 * Probably the better solution here would be to transform that class to Singleton,
 * to not create an instance on every execution
*/

bool is_valid_nickname(const std::string& nick);

typedef int (Executor::*FunctionPointer)(const Message&, User&);

typedef int (Executor::*ModeFunctionPointer)(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);

class Executor
{
private:
	Server* server_;
	std::map<std::string, FunctionPointer> functions_;
	std::map<char, ModeFunctionPointer> mode_functions_;
	std::map<std::string, std::string> params;

	/**
	 * Funcs
	*/
	int pass(const Message& message, User& user); // D
	int nick(const Message& message, User& user); // D
	int user(const Message& message, User& user); // D
	int quit(const Message& message, User& user); // remove user from server
	int ping(const Message& message, User& user); // D : respond to ping
	int pong(const Message& message, User& user); // D : handle incoming pong
	int join(const Message& message, User& user); // add user to channel
	int part(const Message& message, User& user); // remove user from channel
	int names(const Message& message, User& user); // list users in channel
	// int authenticate(const Message& message, User& user); // authenticate user
	int privmsg(const Message& message, User& user); // message user to user/channel
	// int list(const Message& message, User& user); // list channels

	// // operators
	int kick(const Message& message, User& user);
	int invite(const Message& message, User& user);
	int topic(const Message& message, User& user);
	int mode(const Message& message, User& user);

	/**
	 * Mode Funcs
	*/
	int invite_only(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int topic_mode(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int channel_key(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int user_limit(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int channel_operator(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	
public:
	Executor(Server* server);
	~Executor();

	int execute(const Message& message, User& user);
};

#endif // EXECUTOR_HPP
