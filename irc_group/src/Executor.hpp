/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:21:04 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 19:08:15 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "Message.hpp"
#include "User.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "irc_err_codes.hpp"

/**
 * Probably the better solution here would be to transform that class to Singleton,
 * to not create an instance on every execution
*/

bool is_valid_nickname(const std::string& nickname);

typedef int (Executor::*FunctionPointer)(const Message&, User&);

class Executor
{
private:
	Server* server_;
	std::map<std::string, FunctionPointer> functions_;

	int nick(const Message& message, User& user);
	int quit(const Message& message, User& user);

public:
	Executor(Server* server);
	~Executor();

	int execute(const Message& message, User& user);
};




#endif // EXECUTOR_HPP
