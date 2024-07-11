/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:23:07 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/11 19:03:38 by amenses-         ###   ########.fr       */
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
	functions_["PART"] = &Executor::part;
	functions_["NAMES"] = &Executor::names;
	functions_["KICK"] = &Executor::kick;
	functions_["INVITE"] = &Executor::invite;
	functions_["TOPIC"] = &Executor::topic;
	functions_["MODE"] = &Executor::mode;
	functions_["PRIVMSG"] = &Executor::privmsg;

	functions_["QUIT"] = &Executor::quit;

	mode_functions_['i'] = &Executor::invite_only;
	mode_functions_['t'] = &Executor::topic_mode;
	mode_functions_['k'] = &Executor::channel_key;
	mode_functions_['o'] = &Executor::channel_operator;
	mode_functions_['l'] = &Executor::user_limit;
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
	(void)message; // !
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
		user.send_message(":" + server_->get_name() + " PONG " + message.get_arguments()[0] + "\n"); //  ! manage the addition of \n here or in the send_message? function
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
} // maybe usen umap for join? channel,key

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
		std::map<std::string, Channel*>::const_iterator it = server_->get_channels().begin();

		for (; it != server_->get_channels().end(); ++it)
		{
			server_->leave_channel(it->first, user);
		}
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
				if (keys.size() > i)
				{
					server_->join_channel(channel_names[i], keys[i], user);
				}
				else
				{
					server_->join_channel(channel_names[i], "", user);
				}
			}
		}
	}
	return 0;
}

int Executor::part(const Message& message, User& user)
{
	std::vector<std::string> channel_names;
	
	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else 
	{
		channel_names = split_arguments(message.get_arguments()[0]); // maybe just call it channels?
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			if (!server_->contains_channel(channel_names[i]))
			{
				Response::error(user, ERR_NOSUCHCHANNEL, channel_names[i]);
			}
			else if (!server_->user_on_channel(channel_names[i], user))
			{
				Response::error(user, ERR_NOTONCHANNEL, channel_names[i]);
			}
			else
			{
				server_->leave_channel(channel_names[i], user); // ! did not remove channel from User (rethink if its necessary to keep that)
				user.send_message(":" + user.get_prefix() + " PART " + channel_names[i]);
				// MAY broadcast message to channel in addition (horse)
			}
		}
	}
	return 0;
}

int Executor::names(const Message& message, User& user)
{
	std::vector<std::string> channel_names;
	
	if (message.get_arguments().size() == 0)
	{
		// list users in all channels
		server_->list_users("*", user);
	}
	else
	{
		channel_names = split_arguments(message.get_arguments()[0]);
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			if (server_->contains_channel(channel_names[i]))
			{
				// list users in channel
				server_->list_users(channel_names[i], user);
			}
			Response::reply(user, RPL_ENDOFNAMES, channel_names[i]);
		}
	}
	return 0;
}

/**
 * Operator functions
*/

int Executor::kick(const Message& message, User& user) // TARGMAX=KICK:1 !
{
	std::string channel;
	std::string target;
	std::string reply;

	if (message.get_arguments().size() < 2)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else
	{
		channel = message.get_arguments()[0];
		target = message.get_arguments()[1];
		if (!server_->contains_channel(channel))
		{
			Response::error(user, ERR_NOSUCHCHANNEL, channel);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::error(user, ERR_NOTONCHANNEL, channel);
		}
		else if (!server_->user_on_channel(channel, target))
		{
			Response::error(user, ERR_USERNOTINCHANNEL, target, channel);
		}
		else if (!server_->is_operator(channel, user))
		{
			Response::error(user, ERR_CHANOPRIVSNEEDED, channel);
		}
		else
		{
			reply = " KICK " + channel + " " + target; // confirm client prefix is appended
			if (message.get_arguments().size() > 2)
			{
				reply.append(" :" + message.get_arguments()[2] + "\n"); // \r\n ?
			}
			else
			{
				reply.append(" :Kicked\n");
			}
			server_->channel_broadcast(channel, user, reply); // ! check if this is correct (horse) kiscked user also notified right?
			server_->leave_channel(channel, user);
		}
	}
	return 0;
}

int Executor::invite(const Message& message, User& user)
{
	std::string channel;
	std::string target;

	if (message.get_arguments().size() < 2)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else
	{
		channel = message.get_arguments()[0];
		target = message.get_arguments()[1];
		// maybe group all these check in a function that can be used in all exec actions?
		if (!server_->contains_channel(channel))
		{
			Response::error(user, ERR_NOSUCHCHANNEL, channel);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::error(user, ERR_NOTONCHANNEL, channel);
		}
		else if (server_->check_channel_mode(channel, INVITEONLY) && !server_->is_operator(channel, user))
		{
			Response::error(user, ERR_CHANOPRIVSNEEDED, channel);
		}
		else if (!server_->contains_nickname(target))
		{
			Response::error(user, ERR_NOSUCHNICK, target);
		}
		else if (server_->user_on_channel(channel, target))
		{
			Response::error(user, ERR_USERONCHANNEL, target, channel);
		}
		else
		{
			// RPL_INVITING (341) !
			user.send_message(":" + user.get_prefix() + " INVITE " + target + " " + channel);
		}
	}
	return 0;
}

int Executor::topic(const Message& message, User& user)
{
	std::string channel;
	std::string topic;

	if (message.get_arguments().size() < 1)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command());
	}
	else
	{
		channel = message.get_arguments()[0];
		if (!server_->contains_channel(channel))
		{
			Response::error(user, ERR_NOSUCHCHANNEL, channel);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::error(user, ERR_NOTONCHANNEL, channel);
		}
		else if (message.get_arguments().size() == 1)
		{
			// RPL_NOTOPIC (331) or RPL_TOPIC (332) !
			if (server_->get_channel_topic(channel).empty())
			{
				Response::reply(user, RPL_NOTOPIC, channel);
			}
			else
			{
				Response::reply(user, RPL_TOPIC, channel, server_->get_channel_topic(channel));
				Response::reply(user, RPL_TOPICWHOTIME, channel, user.get_prefix());
			}
		}
		else
		{
			topic = message.get_arguments()[1];
			if (server_->check_channel_mode(channel, TOPICMODE) && !server_->is_operator(channel, user))
			{
				Response::error(user, ERR_CHANOPRIVSNEEDED, channel);
			}
			else if (topic.empty())
			{
				server_->get_channels().at(channel)->set_topic(user, "");
			}
			else
			{
				server_->get_channels().at(channel)->set_topic(user, topic);
				server_->get_channels().at(channel)->set_topic_time();
			}
		}
	}
	return 0;
}

std::queue<std::string> vector_to_queue(const std::vector<std::string>& v, size_t i = 0) // UTIL
{
	std::queue<std::string> q;

	for (; i < v.size(); ++i)
	{
		q.push(v[i]);
	}
	return q;
}

int Executor::mode(const Message& message, User& user)
{
	std::string channel;
	std::string modes;
	bool activate = true;
	std::queue<std::string> q_values;

	if (message.get_arguments().size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, message.get_command()); // debatable !
	}
	else
	{
		channel = message.get_arguments()[0];
		if (!server_->contains_channel(channel))
		{
			Response::error(user, ERR_NOSUCHCHANNEL, channel);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::error(user, ERR_NOTONCHANNEL, channel);
		}
		else if (message.get_arguments().size() == 1)
		{
			Response::reply(user, RPL_CHANNELMODEIS, channel); // !
			Response::reply(user, RPL_CREATIONTIME, channel); // !
		}
		else
		{
			modes = message.get_arguments()[1];
			modes[0] == '-' ? activate = false : activate = true;
			if (modes[0] == '+' || modes[0] == '-')
			{
				modes = modes.substr(1);
			}
			if (message.get_arguments().size() > 2)
			{
				q_values = vector_to_queue(message.get_arguments(), 2);
			}
			for (size_t i = 0; i < modes.size(); ++i)
			{
				if (mode_functions_.find(modes[i]) != mode_functions_.end())
				{
					ModeFunctionPointer mfp = NULL;
					
					if (modes[i] == 'k' || modes[i] == 'o')
					{
						mfp = mode_functions_.at('B');
					}
					else if (modes[i] == 'l')
					{
						mfp = mode_functions_.at('C');
					}
					else if (modes[i] == 'i' || modes[i] == 't')
					{
						mfp = mode_functions_.at('D');
					}
					if (mfp)
					{
						(this->*mfp)(channel, user, q_values, activate);
					}
				}
				else
				{
					Response::error(user, ERR_UNKNOWNMODE, "what?"); // confirm ! also not sure it's needed for each... !
				}
			}
		}
	}
	return 0;
}

int Executor::invite_only(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	(void)user;
	(void)q_values;
	if (activate)
	{
		server_->get_channels().at(channel)->set_flag(INVITEONLY);
	}
	else
	{
		server_->get_channels().at(channel)->reset_flag(INVITEONLY);
	}
	return 0;
}

int Executor::topic_mode(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	(void)user;
	(void)q_values;
	if (activate)
	{
		server_->get_channels().at(channel)->set_flag(TOPICMODE);
	}
	else
	{
		server_->get_channels().at(channel)->reset_flag(TOPICMODE);
	}
	return 0;
}

int Executor::channel_key(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	// double check JOIN command for key !
	std::string key;

	if (q_values.size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, "MODE");
	}
	else
	{
		key = q_values.front();
		q_values.pop();
		if (activate)
		{
			if (server_->get_channels().at(channel)->get_flags() & CHANNELKEY)
			{
				Response::error(user, ERR_KEYSET, channel);
			}
			server_->get_channels().at(channel)->set_flag(CHANNELKEY);
			server_->get_channels().at(channel)->set_password(user, key);
		}
		else
		{
			if (key != server_->get_channels().at(channel)->get_password())
			{
				Response::error(user, ERR_KEYSET, channel); // confirm !
			}
			else
			{
				server_->get_channels().at(channel)->set_password(user, "");
				server_->get_channels().at(channel)->reset_flag(CHANNELKEY);
			}
		}
	}
	return 0;
}

int Executor::user_limit(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	// double check JOIN command for user limit !
	if (q_values.size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, "MODE");
	}
	else
	{
		std::istringstream iss(q_values.front());
		unsigned short value = 0;

		iss >> value;
		if (iss.fail() || value == 0)
		{
			Response::error(user, ERR_NEEDMOREPARAMS, "MODE"); // confirm ! seems right though
		}
		else if (activate)
		{
			server_->get_channels().at(channel)->set_flag(USERLIMIT); // necessary or redundant? better to keep to be able to show active modes
			server_->get_channels().at(channel)->set_user_limit(value);
		}
		else
		{
			server_->get_channels().at(channel)->reset_flag(USERLIMIT);
			server_->get_channels().at(channel)->set_user_limit(0);
		}
	}
	return 0;
}

int Executor::channel_operator(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
// need to set flag? what if there are many operators, when to reset?
{
	std::string target_nick;

	if (q_values.size() == 0)
	{
		Response::error(user, ERR_NEEDMOREPARAMS, "MODE");
	}
	else
	{
		target_nick = q_values.front();
		q_values.pop();
		if (!server_->get_channels().at(channel)->contains_nickname(target_nick))
		{
			Response::error(user, ERR_NOSUCHNICK, target_nick);
		}
		else if (activate)
		{
			if (!server_->get_channels().at(channel)->is_operator(target_nick))
			{
				server_->get_channels().at(channel)->add_operator(target_nick);
			}
		}
		else
		{
			if (server_->get_channels().at(channel)->is_operator(target_nick))
			{
				server_->get_channels().at(channel)->remove_operator(target_nick);
			}
		}
	}
	return 0;
}

int Executor::privmsg(const Message& message, User& user)
{
	std::string target;
	std::string message_text;

	if (message.get_arguments().size() < 2)
	{
		if (message.get_arguments().size() == 1 && message.get_trailing_flag())
		{
			Response::error(user, ERR_NOTEXTTOSEND, message.get_command());
		}
		else
		{
			Response::error(user, ERR_NORECIPIENT, message.get_command());
		}
	}
	else
	{
		target = message.get_arguments()[0];
		message_text = message.get_arguments()[1];
		if (target[0] == '#') // channel
		{
			if (!server_->contains_channel(target))
			{
				Response::error(user, ERR_NOSUCHCHANNEL, target);
			}
			else if (!server_->user_on_channel(target, user))
			{
				Response::error(user, ERR_CANNOTSENDTOCHAN, target);
			}
			else
			{
				server_->channel_broadcast(target, user, ":" + user.get_prefix() + " PRIVMSG " + target + " :" + message_text + "\n");
			}
		}
		else // user
		{
			if (!server_->contains_nickname(target))
			{
				Response::error(user, ERR_NOSUCHNICK, target);
			}
			else
			{
				server_->get_user(target)->send_message(":" + user.get_prefix() + " PRIVMSG " + target + " :" + message_text + "\n");
			}
		}
	}
	return 0;
}
