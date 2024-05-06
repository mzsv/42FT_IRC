/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:23:07 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 19:09:00 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executor.hpp"

Executor::Executor(Server* server) : server_(server)
{
	functions_["QUIT"] = &Executor::quit;
	functions_["NICK"] = &Executor::nick;
}

Executor::~Executor()
{
}

int Executor::execute(const Message& message, User& user)
{
	Logger::Log(DEBUG, message.get_command());
	FunctionPointer fp = functions_.at(message.get_command());
	return (this->*fp)(message, user);
}

int Executor::quit(const Message& message, User& user)
{
	(void)message;
	(void)user;
	return 0;
}


int Executor::nick(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else if (!is_valid_nickname(message.get_arguments()[0]) || message.get_arguments()[0] == server_->get_name())
	{
		Response::error(user, ERR_ERRONEUSNICKNAME, message.get_arguments()[0]);
	}
	else if (server_->contains_nickname(message.get_arguments()[0]))
	{
		Response::error(user, ERR_NICKNAMEINUSE, message.get_arguments()[0]);
	}
	else
	{
		if (user.get_flags() & REGISTERED)
		{
			server_->notify_users(user, ":" + user.get_prefix() + " " + message.get_command() + " " + message.get_arguments()[0] + "\n");
			// add to history
		}
		user.set_nickname(message.get_arguments()[0]);
	}
	return server_->check_connection(user);
}
