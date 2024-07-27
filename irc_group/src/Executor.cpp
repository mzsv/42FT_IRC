/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:23:07 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/27 16:40:42 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executor.hpp"

Executor::Executor(Server* server) : server_(server)
{
	/* Command Handlers */
	functions_["CAP"] = &Executor::ignore;
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
	functions_["NOTICE"] = &Executor::notice;
	functions_["MOTD"] = &Executor::motd;
	functions_["LUSERS"] = &Executor::lusers;
	functions_["WHO"] = &Executor::who;
	functions_["WHOIS"] = &Executor::whois;
	functions_["QUIT"] = &Executor::quit;

	/* Mode Handlers */
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
	FunctionPointer fp;
	Response::add_param("command", message.get_command()); // !
	Response::set_command(message.get_command());
	Response::set_user(&user);
	try
	{
		fp = functions_.at(message.get_command());
		return (this->*fp)(message, user);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		Response::num_reply(ERR_UNKNOWNCOMMAND);
	}
	return 0;
}

int Executor::quit(const Message& message, User& user)
{
	std::string reason = ":Quit: ";
	std::vector<const Channel*> channels = user.get_channels();

	if (message.get_arguments().size() > 0)
	{
		reason += message.get_arguments()[0];
	}
	Response::add_param("reason", reason);
	return DISCONNECT;
}

int Executor::pass(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else if (user.get_flags() & REGISTERED)
	{
		Response::num_reply(ERR_ALREADYREGISTERED);
	}
	else if (message.get_arguments()[0] != server_->get_password())
	{
		Response::num_reply(ERR_PASSWDMISMATCH);
	}
	else
	{
		user.set_password(message.get_arguments()[0]);
	}
	return server_->check_connection(user);
}

int Executor::nick(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else if (!is_valid_nickname(message.get_arguments()[0]) || message.get_arguments()[0] == server_->get_name())
	{
		Response::add_param("nickname", message.get_arguments()[0]);
		Response::num_reply(ERR_ERRONEUSNICKNAME);
	}
	else if (server_->contains_nickname(message.get_arguments()[0]))
	{
		Response::add_param("nickname", message.get_arguments()[0]);
		Response::num_reply(ERR_NICKNAMEINUSE);
	}
	else
	{
		std::string new_nick = message.get_arguments()[0];
		if (user.get_flags() & REGISTERED)
		{
			server_->notify_users(user, ":" + user.get_prefix() + " NICK " + new_nick + "\r\n");
		}
		user.set_nickname(message.get_arguments()[0]);
	}
	return server_->check_connection(user);
}

int Executor::user(const Message& message, User& user)
{
	if (message.get_arguments().size() < 4)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else if (user.get_flags() & REGISTERED)
	{
		Response::num_reply(ERR_ALREADYREGISTERED);
	}
	else
	{
		std::string username = message.get_arguments()[0];
		size_t max_len = ston<size_t>(server_->get_isupport_param("USERLEN"));
		if (is_valid_username(username))
		{
			if (username.size() > max_len)
			{
				username = username.substr(0, max_len);
			}
		}
		else
		{
			if (user.get_nickname().size())
			{
				username = user.get_nickname();
			}
			else
			{
				username = "default";
			}
		}
		user.set_username(username);
		user.set_realname(message.get_arguments()[3]);
	}
	return server_->check_connection(user);
}

int Executor::ping(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NOORIGIN);
	}
	else
	{
		Response::add_param("server", server_->get_name());
		Response::add_param("ping_token", message.get_arguments()[0]);
		Response::cmd_reply(CMD_PONG, NULL, user);
	}
	return 0;
}

int Executor::pong(const Message& message, User& user)
{
	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NOORIGIN);
	}
	else if (user.get_flags() & PINGING)
	{
		if (message.get_arguments()[0] == user.get_server_name())
		{
			user.set_time_after_pinging();
			user.set_time_of_last_action();
			user.reset_flag(PINGING);
		}
	}
	return 0;
}

std::vector<std::string> split_arguments(const std::string& arguments)
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

	if (!(user.get_flags() & REGISTERED))
	{
		Response::num_reply(ERR_NOTREGISTERED);
	}
	else if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else if (message.get_arguments()[0] == to_string_(0))
	{
		std::map<std::string, Channel*>::const_iterator it = server_->get_channels().begin();

		for (; it != server_->get_channels().end(); ++it)
		{
			Response::add_param("channel", it->first);
			Response::add_param("reason", "");
			Response::cmd_reply(CMD_PART, user, *it->second);
			server_->leave_channel(it->first, user);
		}
	}
	else
	{
		channel_names = split_arguments(message.get_arguments()[0]);
		size_t max_len = ston<size_t>(server_->get_isupport_param("CHANNELLEN"));

		if (message.get_arguments().size() > 1)
		{
			keys = split_arguments(message.get_arguments()[1]);
		}
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			Response::add_param("channel", channel_names[i]);
			if (channel_names[i].size() > max_len || !is_valid_channel(channel_names[i]))
			{
				Response::num_reply(ERR_BADCHANMASK);
			}
			else if (!(server_->contains_channel(channel_names[i]) && \
					server_->user_on_channel(channel_names[i], user)))
			{
				if (keys.size() > i)
				{
					server_->join_channel(channel_names[i], keys[i], user);
				}
				else
				{
					server_->join_channel(channel_names[i], "", user);
				}
				Response::set_channel(user.get_server()->get_channels().at(channel_names[i]));
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
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else 
	{
		channel_names = split_arguments(message.get_arguments()[0]);
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			Response::add_param("channel", channel_names[i]);
			if (!server_->contains_channel(channel_names[i]))
			{
				Response::num_reply(ERR_NOSUCHCHANNEL);
			}
			else if (!server_->user_on_channel(channel_names[i], user))
			{
				Response::num_reply(ERR_NOTONCHANNEL);
			}
			else
			{
				std::string reason;
				if (message.get_arguments().size() > 1)
				{
					reason = ":" + message.get_arguments()[1];
				}
				Response::add_param("reason", reason);
				Response::cmd_reply(CMD_PART, user, *server_->get_channels().at(channel_names[i]));
				server_->leave_channel(channel_names[i], user);
			}
		}
	}
	return 0;
}

int Executor::names(const Message& message, User& user)
{
	std::vector<std::string> channel_names;

	Response::add_param("channel", "");
	if (message.get_arguments().size() == 0)
	{
		std::vector<const Channel*> channels = user.get_channels();
		for (size_t i = 0; i < channels.size(); ++i)
		{
			Response::set_channel(channels[i]);
			Response::num_reply(RPL_NAMREPLY);
		}
		Response::num_reply(RPL_ENDOFNAMES);
	}
	else
	{
		channel_names = split_arguments(message.get_arguments()[0]);
		for (size_t i = 0; i < channel_names.size(); ++i)
		{
			Response::add_param("channel", channel_names[i]);
			if (server_->contains_channel(channel_names[i]))
			{
				Response::set_channel(server_->get_channels().at(channel_names[i]));
				Response::num_reply(RPL_NAMREPLY);
			}
			Response::num_reply(RPL_ENDOFNAMES);
		}
	}
	return 0;
}

/**
 * Operator functions
*/
int Executor::kick(const Message& message, User& user)
{
	std::string channel;
	std::string target;
	std::string reply;

	if (message.get_arguments().size() < 2)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		channel = message.get_arguments()[0];
		target = message.get_arguments()[1];
		Response::add_param("channel", channel);
		Response::add_param("nickname", user.get_nickname());
		Response::add_param("target_nickname", target);
		if (!server_->contains_channel(channel))
		{
			Response::num_reply(ERR_NOSUCHCHANNEL);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::num_reply(ERR_NOTONCHANNEL);
		}
		else if (!server_->user_on_channel(channel, target))
		{
			Response::num_reply(ERR_USERNOTINCHANNEL);
		}
		else if (!server_->is_operator(channel, user))
		{
			Response::num_reply(ERR_CHANOPRIVSNEEDED);
		}
		else
		{
			User* target_user = server_->get_user(target);
			Response::add_param("reason", ":Kicked");
			if (message.get_arguments().size() > 2)
			{
				Response::add_param("reason", ":" + message.get_arguments()[2]);
			}
			Response::cmd_reply(CMD_KICK, user, *server_->get_channels().at(channel));
			server_->leave_channel(channel, *target_user);
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
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		target = message.get_arguments()[0];
		channel = message.get_arguments()[1];
		Response::add_param("nickname", user.get_nickname());
		Response::add_param("channel", channel);
		Response::add_param("target_nickname", target);
		if (!server_->contains_channel(channel))
		{
			Response::num_reply(ERR_NOSUCHCHANNEL);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::num_reply(ERR_NOTONCHANNEL);
		}
		else if (server_->check_channel_mode(channel, INVITEONLY) && !server_->is_operator(channel, user))
		{
			Response::num_reply(ERR_CHANOPRIVSNEEDED);
		}
		else if (!server_->contains_nickname(target))
		{
			Response::num_reply(ERR_NOSUCHNICK);
		}
		else if (server_->user_on_channel(channel, target))
		{
			Response::num_reply(ERR_USERONCHANNEL);
		}
		else
		{
			const User* target_ptr = server_->get_user(target);
			
			Response::set_channel(server_->get_channels().at(channel));
			Response::set_target_user(target_ptr);
			Response::num_reply(RPL_INVITING);
			Response::cmd_reply(CMD_INVITE, &user, *target_ptr);
			server_->get_channels().at(channel)->add_invite(target_ptr);
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
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		channel = message.get_arguments()[0];
		Response::add_param("channel", channel);
		Response::add_param("nickname", user.get_nickname());
		if (!server_->contains_channel(channel))
		{
			Response::num_reply(ERR_NOSUCHCHANNEL);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::num_reply(ERR_NOTONCHANNEL);
		}
		else if (message.get_arguments().size() == 1)
		{
			if (server_->get_channel_topic(channel).empty())
			{
				Response::num_reply(RPL_NOTOPIC);
			}
			else
			{
				Response::set_channel(server_->get_channels().at(channel));
				Response::num_reply(RPL_TOPIC);
				Response::num_reply(RPL_TOPICWHOTIME);
			}
		}
		else
		{
			Channel* channel_ptr = server_->get_channels().at(channel);
			topic = message.get_arguments()[1];
			Response::add_param("topic", topic);
			if (server_->check_channel_mode(channel, TOPICMODE) && !channel_ptr->is_operator(user))
			{
				Response::num_reply(ERR_CHANOPRIVSNEEDED);
				return 0;
			}
			else if (topic.empty())
			{
				channel_ptr->set_topic(user, "");
			}
			else
			{
				channel_ptr->set_topic(user, topic);
				channel_ptr->set_topic_time();
			}
			Response::cmd_reply(CMD_TOPIC, user, *channel_ptr);
		}
	}
	return 0;
}

std::queue<std::string> vector_to_queue(const std::vector<std::string>& v, size_t i = 0)
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
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		channel = message.get_arguments()[0];
		Response::add_param("channel", channel);
		Response::add_param("nickname", user.get_nickname());
		if (!server_->contains_channel(channel))
		{
			Response::num_reply(ERR_NOSUCHCHANNEL);
		}
		else if (!server_->user_on_channel(channel, user))
		{
			Response::num_reply(ERR_NOTONCHANNEL);
		}
		else if (message.get_arguments().size() == 1)
		{
			Response::set_channel(server_->get_channels().at(channel));
			Response::num_reply(RPL_CHANNELMODEIS);
			Response::num_reply(RPL_CREATIONTIME);
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
				Response::add_param("mode", modes.substr(i, 1));
				if (mode_functions_.find(modes[i]) != mode_functions_.end())
				{
					ModeFunctionPointer mfp = mode_functions_.at(modes[i]);
					(this->*mfp)(channel, user, q_values, activate);
				}
				else
				{
					Response::num_reply(ERR_UNKNOWNMODE);
				}
			}
		}
	}
	return 0;
}

int Executor::invite_only(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	Channel* channel_ptr = server_->get_channels().at(channel);
	std::string mode_str = activate ? "+i" : "-i";
	unsigned char flag = (channel_ptr->get_flags() & INVITEONLY);

	(void)user;
	(void)q_values;
	if (activate && !(channel_ptr->get_flags() & INVITEONLY))
	{
		channel_ptr->set_flag(INVITEONLY);
	}
	else if (!activate && (channel_ptr->get_flags() & INVITEONLY))
	{
		channel_ptr->reset_flag(INVITEONLY);
	}
	if (flag != (channel_ptr->get_flags() & INVITEONLY))
	{
		channel_ptr->clear_invites();
		Response::add_param("mode", mode_str);
		Response::cmd_reply(CMD_MODE, user, *channel_ptr);
	}
	return 0;
}

int Executor::topic_mode(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	Channel* channel_ptr = server_->get_channels().at(channel);
	std::string mode_str = activate ? "+t" : "-t";
	unsigned char flag = (channel_ptr->get_flags() & TOPICMODE);

	(void)user;
	(void)q_values;
	if (activate && !(channel_ptr->get_flags() & TOPICMODE))
	{
		channel_ptr->set_flag(TOPICMODE);
	}
	else if (!activate && (channel_ptr->get_flags() & TOPICMODE))
	{
		channel_ptr->reset_flag(TOPICMODE);
	}
	if (flag != (channel_ptr->get_flags() & TOPICMODE))
	{
		Response::add_param("mode", mode_str);
		Response::cmd_reply(CMD_MODE, user, *channel_ptr);
	}
	return 0;
}

int Executor::channel_key(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	Channel* channel_ptr = server_->get_channels().at(channel);
	std::string key;
	std::string mode_str = activate ? "+k" : "-k";
	unsigned char flag = (channel_ptr->get_flags() & CHANNELKEY);

	if (q_values.size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		key = q_values.front();
		q_values.pop();
		Response::add_param("value", key);
		if (!is_valid_key(key))
		{
			Response::num_reply(ERR_INVALIDMODEPARAM);
		}
		else if (activate)
		{
			if (!(channel_ptr->get_flags() & CHANNELKEY))
			{
				channel_ptr->set_flag(CHANNELKEY);
				channel_ptr->set_password(user, key);
			}
		}
		else if (channel_ptr->get_flags() & CHANNELKEY)
		{
			if (key != channel_ptr->get_password())
			{
				Response::num_reply(ERR_KEYSET);
			}
			else
			{
				channel_ptr->set_password(user, "");
				channel_ptr->reset_flag(CHANNELKEY);
			}
		}
		if (flag != (channel_ptr->get_flags() & CHANNELKEY))
		{
			Response::add_param("mode_str", mode_str + " " + key);
			Response::cmd_reply(CMD_MODE, user, *channel_ptr);
		}
	}
	return 0;
}

int Executor::user_limit(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	Channel* channel_ptr = server_->get_channels().at(channel);
	std::string mode_str = activate ? "+l" : "-l";
	unsigned short value = 0;

	(void)user;
	if (activate)
	{
		if (q_values.size() == 0)
		{
			Response::num_reply(ERR_NEEDMOREPARAMS);
			return -1;
		}
		std::istringstream iss(q_values.front());
		Response::add_param("value", q_values.front());
		q_values.pop();
		iss >> value;
		if (iss.fail() || value == 0)
		{
			Response::num_reply(ERR_INVALIDMODEPARAM);
			return -1; // ?
		}
		channel_ptr->set_flag(USERLIMIT);
		channel_ptr->set_user_limit(value);
	}
	else
	{
		channel_ptr->reset_flag(USERLIMIT);
		channel_ptr->set_user_limit(0);
	}
	Response::add_param("mode_str", mode_str + " " + to_string_(value));
	Response::cmd_reply(CMD_MODE, user, *channel_ptr);
	return 0;
}

int Executor::channel_operator(std::string channel, User& user, std::queue<std::string>& q_values, bool activate)
{
	Channel* channel_ptr = server_->get_channels().at(channel);
	std::string target_nick;
	std::string mode_str = activate ? "+o" : "-o";
	bool is_operator = false;

	(void)user;
	if (q_values.size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		target_nick = q_values.front();
		is_operator = channel_ptr->is_operator(target_nick);
		q_values.pop();
		if (!channel_ptr->contains_nickname(target_nick))
		{
			Response::add_param("nickname", target_nick);
			Response::num_reply(ERR_NOSUCHNICK);
		}
		else if (activate)
		{
			if (!is_operator)
			{
				channel_ptr->add_operator(target_nick);
			}
		}
		else
		{
			if (is_operator)
			{
				channel_ptr->remove_operator(target_nick);
			}
		}
		if (is_operator != channel_ptr->is_operator(target_nick))
		{
			Response::add_param("mode_str", mode_str + " " + target_nick);
			Response::cmd_reply(CMD_MODE, user, *channel_ptr);
		}
	}
	return 0;
}

int Executor::privmsg(const Message& message, User& user)
{
	std::string target;
	std::string message_text;
	std::string command = message.get_command();

	if (message.get_arguments().size() < 2)
	{
		if (message.get_arguments().size() == 1 && !message.get_trailing_flag())
		{
			if (command != "NOTICE")
			{
				Response::num_reply(ERR_NOTEXTTOSEND);
			}
		}
		else if (command != "NOTICE")
		{
			Response::num_reply(ERR_NORECIPIENT);
		}
	}
	else
	{
		target = message.get_arguments()[0];
		message_text = message.get_arguments()[1];
		Response::add_param("target", target);
		Response::add_param("message", message_text);
		if (target[0] == '#')
		{
			Response::add_param("channel", target);
			if (!server_->contains_channel(target))
			{
				if (command != "NOTICE")
				{
					Response::num_reply(ERR_NOSUCHCHANNEL);
				}
			}
			else if (!server_->user_on_channel(target, user))
			{
				if (command != "NOTICE")
				{
					Response::num_reply(ERR_CANNOTSENDTOCHAN);
				}
			}
			else
			{
				if (command == "PRIVMSG")
				{
					Response::cmd_reply(CMD_PRIVMSG, user, *server_->get_channels().at(target), false);
				}
				else
				{
					Response::cmd_reply(CMD_NOTICE, user, *server_->get_channels().at(target), false);
				}
			}
		}
		else
		{
			Response::add_param("nickname", target);
			if (!server_->contains_nickname(target))
			{
				if (command != "NOTICE")
				{
					Response::num_reply(ERR_NOSUCHNICK);
				}
			}
			else
			{
				if (command == "PRIVMSG")
				{
					Response::cmd_reply(CMD_PRIVMSG, &user, *server_->get_user(target));
				}
				else
				{
					Response::cmd_reply(CMD_NOTICE, &user, *server_->get_user(target));
				}
			}
		}
	}
	return 0;
}

int Executor::notice(const Message& message, User& user)
{
	return privmsg(message, user);
}

int Executor::motd(const Message& message, User& user)
{
	std::string filename = "MOTD.txt";
	std::ifstream motd_file(filename.c_str(), std::ios_base::in);
	std::string line;

	(void)message;
	(void)user;
    if (!motd_file)
	{
		Response::num_reply(ERR_NOMOTD);
		return 0;
    }
	Response::num_reply(RPL_MOTDSTART);
	while (std::getline(motd_file, line))
	{
		if (line.size() > 0)
		{
			Response::add_param("message_of_the_day", line);
			Response::num_reply(RPL_MOTD);
		}
	}
	Response::num_reply(RPL_ENDOFMOTD);
	return 0;
}

int Executor::lusers(const Message& message, User& user)
{
	(void)message;
	(void)user;
	Response::num_reply(RPL_LUSERCLIENT);
	Response::num_reply(RPL_LUSEROP);
	Response::num_reply(RPL_LUSERUNKNOWN);
	Response::num_reply(RPL_LUSERCHANNELS);
	Response::num_reply(RPL_LUSERME);
	Response::num_reply(RPL_LOCALUSERS);
	Response::num_reply(RPL_GLOBALUSERS);
	return 0;
}

int Executor::ignore(const Message& message, User& user)
{
	(void)message;
	(void)user;
	return 0;
}

int Executor::who(const Message& message, User& user)
{
	std::string mask;

	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NEEDMOREPARAMS);
	}
	else
	{
		mask = message.get_arguments()[0];
		Response::add_param("who_mask", mask);
		if (mask[0] == '#')
		{
			if (server_->contains_channel(mask) && server_->user_on_channel(mask, user))
			{
				const Channel* channel = server_->get_channels().at(mask);
				const std::vector<const User*> channel_users = channel->get_users();
				Response::set_channel(channel);
				for (size_t i = 0; i < channel_users.size(); ++i)
				{
					Response::set_target_user(channel_users[i]);
					Response::num_reply(RPL_WHOREPLY);
				}
			}
		}
		else if (server_->contains_nickname(mask))
		{
			const User* target = server_->get_user(mask);
			Response::set_target_user(target);
			if (target->get_channels().size())
			{
				Response::set_channel(target->get_channels()[0]);
			}
			Response::num_reply(RPL_WHOREPLY);
		}
		Response::num_reply(RPL_ENDOFWHO);
	}
	return 0;
}

int Executor::whois(const Message& message, User& user)
{
	(void)user;
	if (message.get_arguments().size() == 0)
	{
		Response::num_reply(ERR_NONICKNAMEGIVEN);
	}
	else
	{
		std::string mask;
		if (message.get_arguments().size() > 1)
		{
			mask = message.get_arguments()[0];
		}
		else
		{
			mask = message.get_arguments()[1];
		}
		Response::add_param("whois_mask", mask);
		if (server_->contains_nickname(mask))
		{
			Response::num_reply(RPL_WHOISUSER);
			Response::num_reply(RPL_WHOISHOST);
			Response::num_reply(RPL_WHOISSERVER);
			Response::num_reply(RPL_WHOISMODES);
			Response::num_reply(RPL_WHOISIDLE);
			Response::num_reply(RPL_ENDOFWHOIS);
		}
		else
		{
			Response::num_reply(ERR_NOSUCHNICK);
		}
	}
	return 0;
}
