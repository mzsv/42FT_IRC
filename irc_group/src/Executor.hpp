/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:21:04 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/26 19:06:47 by amenses-         ###   ########.fr       */
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

bool is_valid_nickname(const std::string& nick);

bool is_valid_channel(const std::string& channel);

bool is_valid_key(const std::string& key);

bool is_valid_username(const std::string& username);

typedef int (Executor::*FunctionPointer)(const Message&, User&);

typedef int (Executor::*ModeFunctionPointer)(std::string, User&, std::queue<std::string>&, bool);

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
	/* IRC Commands */
	int pass(const Message& message, User& user);
	int nick(const Message& message, User& user);
	int user(const Message& message, User& user);
	int quit(const Message& message, User& user);
	int ping(const Message& message, User& user);
	int pong(const Message& message, User& user);
	int join(const Message& message, User& user);
	int part(const Message& message, User& user);
	int names(const Message& message, User& user);
	int privmsg(const Message& message, User& user);
	int notice(const Message& message, User& user);
	int motd(const Message& message, User& user);
	int lusers(const Message& message, User& user);
	int ignore(const Message& message, User& user);
	int who(const Message& message, User& user);
	int whois(const Message& message, User& user);
	
	/* IRC Channel Operator Commands */
	int kick(const Message& message, User& user);
	int invite(const Message& message, User& user);
	int topic(const Message& message, User& user);
	int mode(const Message& message, User& user);

	/* IRC Channel Modes */
	int invite_only(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int topic_mode(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int channel_key(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int user_limit(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	int channel_operator(std::string channel, User& user, std::queue<std::string>& q_values, bool activate);
	
	/**
     * Forbidden APIs
    */
   	Executor();
	Executor(const Executor& obj);
	Executor& operator=(const Executor& obj);

public:
	Executor(Server* server);
	~Executor();

	int execute(const Message& message, User& user);
};

#endif // EXECUTOR_HPP
