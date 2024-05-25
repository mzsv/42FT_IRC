/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:23:07 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/25 21:35:23 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executor.hpp"

Executor::Executor(Server* server) : server_(server)
{
	functions_["PASS"] = &Executor::pass;
	functions_["NICK"] = &Executor::nick;
	functions_["USER"] = &Executor::user;
	functions_["PING"] = &Executor::ping;
	functions_["PONG"] = &Executor::pong;
	functions_["JOIN"] = &Executor::join;
	// functions_["PART"] = &Executor::part;
	
	functions_["QUIT"] = &Executor::quit;
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

int Executor::pass(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else if (user.get_flags() & REGISTERED)
	{
		Response::error(user, ERR_ALREADYREGISTERED);
	}
	else if (message.get_arguments()[0] != server_->get_password())
	{
		Response::error(user, ERR_PASSWDMISMATCH);
	}
	else
	{
		user.set_password(message.get_arguments()[0]);
	}
	return server_->check_connection(user); // necessary? or redundant?
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

int Executor::user(const Message& message, User& user)
{
	if (message.get_arguments().size() < 4)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else if (user.get_flags() & REGISTERED)
	{
		Response::error(user, ERR_ALREADYREGISTERED);
	}
	else
	{
		user.set_username(message.get_arguments()[0]);
	}
	return server_->check_connection(user); // necessary? or redundant?
}

int Executor::ping(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else
	{
		user.send_message(":" + server_->get_name() + " PONG " + message.get_arguments()[0]);
	}
	return 0;
}

int Executor::pong(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else
	{
		// should we confirm the pong message? maybe not, for flexibility?
		user.set_time_after_pinging();
		user.set_time_of_last_action();
		user.reset_flag(PINGING);
	}
	return 0;
}

std::vector<std::string> split_arguments(const std::string& arguments) // util !
{
	std::vector<std::string> result;
	std::istringstream iss(arguments);
	std::string token;
	
	while (std::getline(iss, token, ','))
	{
		result.push_back(token);
	}
	return result;
}

int Executor::join(const Message& message, User& user)
{
	std::vector<std::string> channel_names;
	std::vector<std::string> keys;

	if (user.get_flags() & REGISTERED)
	{
		Response::error(user, ERR_NOTREGISTERED);
	}
	else if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else if (message.get_arguments()[0] == "0")
	{
		// leave/PART all channels !
	}
	else 
	{
		channel_names = split_arguments(message.get_arguments()[0]);
		if (message.get_arguments().size() > 1)
		{
			keys = split_arguments(message.get_arguments()[1]);
		}	
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			if (channel_names[i][0] != '#')
			{
				Response::error(user, ERR_BADCHANMASK, message.get_arguments()[0]);
			}
			else
			{
				server_->join_channel(channel_names[i], user);
			}
		}
	}
	return 0;
}
