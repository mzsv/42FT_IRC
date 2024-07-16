/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:55:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/16 20:28:51 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

User* Response::user_ = NULL;

Channel* Response::channel_ = NULL;

std::string Response::command_; // ?

std::map<IrcCode, std::string> Response::irc_messages_ = Response::initialize_irc_messages();

std::map<std::string, std::string> Response::params_;

std::map<IrcCode, RplFunctionPointer> Response::rpl_functions_ = initialize_rpl_functions();

std::map<IrcCode, std::string> Response::initialize_irc_messages()
{
	std::map<IrcCode, std::string> messages;

	// Command Responses
	messages[RPL_WELCOME] = ":Welcome to the {server_nick} Network, {client_prefix}"; // client_prefix = nickname!username@hostname
	messages[RPL_YOURHOST] = ":Your host is {server_name}, running version {server_version}";
	messages[RPL_CREATED] = ":This server was created {server_date}";
	messages[RPL_MYINFO] = "{server_name} {server_version} {channel_modes}"; // no user modes
	messages[RPL_ISUPPORT] = "{tokens} :are supported by this server"; // tokens = "CHANTYPES=# PREFIX=(ov)@+ MODES=3 CHANLIMIT=#:10 NICKLEN=9 TOPICLEN=120 KICKLEN=120 CHANNELLEN=50 CHANMODES=beI,k,l,imnpstz";
	messages[RPL_LUSERCLIENT] = ":There are {n_clients} users and {n_servers} servers";
	messages[RPL_LUSEROP] = "{n_operators} :operator(s) online";
	messages[RPL_LUSERUNKNOWN] = "{n_unknown} :unknown connection(s)";
	messages[RPL_LUSERCHANNELS] = "{n_channels} :channels formed";
	messages[RPL_LUSERME] = ":I have {n_clients} clients and {n_servers} servers";
	messages[RPL_LOCALUSERS] = "{n_local} {n_local_max} :Current local users {n_local}, max {n_local_max}";
	messages[RPL_GLOBALUSERS] = "{n_global} {n_global_max} :Current global users {n_global}, max {n_global_max}";
	messages[RPL_CHANNELMODEIS] = "{channel} {activated_modes} {mode_params}";
	messages[RPL_CREATIONTIME] = "{channel} {ch_timestamp} :Channel created at {ch_timestamp}";
	messages[RPL_NOTOPIC] = "{channel} :No topic is set";
	messages[RPL_TOPIC] = "{channel} :{topic}";
	messages[RPL_TOPICWHOTIME] = "{channel} {nickname} {t_timestamp} :{topic}";
	messages[RPL_INVITING] = "{nickname} {channel} :Inviting {target_nickname} to {channel}";
	messages[RPL_NAMREPLY] = "={channel} :{nicknames}"; // NOT REQUIRED ! nicknames = "@nickname1 +nickname2 nickname3" @ for op, + for voice
	messages[RPL_ENDOFNAMES] = "{channel} :End of /NAMES list";
	messages[RPL_MOTD] = ":- {message_of_the_day}";
	messages[RPL_MOTDSTART] = ":- {server_name} Message of the Day - ";
	messages[RPL_ENDOFMOTD] = ":End of /MOTD command";

	// Error Responses
	messages[ERR_NOSUCHNICK] = "{nickname} :No such nick/channel";
	messages[ERR_NOSUCHCHANNEL] = "{channel} :No such channel";
	messages[ERR_CANNOTSENDTOCHAN] = "{channel} :Cannot send to channel";
	messages[ERR_NOORIGIN] = ":No origin specified";
	messages[ERR_NORECIPIENT] = ":No recipient given ({command})";
	messages[ERR_NOTEXTTOSEND] = ":No text to send";
	messages[ERR_UNKNOWNCOMMAND] = "{command} :Unknown command";
	messages[ERR_USERNOTINCHANNEL] = "{target_nickname} :is not on channel {channel}";
	messages[ERR_NOTONCHANNEL] = "{nickname} :is not on channel {channel}";
	messages[ERR_USERONCHANNEL] = "{target_nickname} :is already on channel {channel}";
	messages[ERR_NOTREGISTERED] = ":You have not registered";
	messages[ERR_CHANNELISFULL] = "{channel} :Cannot join channel (+l)";
	messages[ERR_INVITEONLYCHAN] = "{channel} :Cannot join channel (+i)";
	messages[ERR_BADCHANNELKEY] = "{channel} :Cannot join channel (+k)";
	messages[ERR_BADCHANMASK] = "{channel} :Bad channel mask";
	messages[ERR_CHANOPRIVSNEEDED] = "{channel} :You're not channel operator";
	messages[ERR_NEEDMOREPARAMS] = "{command} :Not enough parameters";
	messages[ERR_ALREADYREGISTERED] = ":You may not reregister";
	messages[ERR_PASSWDMISMATCH] = ":Password incorrect";
	messages[ERR_ERRONEUSNICKNAME] = "{nickname} :Erroneous nickname";
	messages[ERR_NICKNAMEINUSE] = "{nickname} :Nickname is already in use";
	messages[ERR_KEYSET] = "{channel} :Channel key already set";
	messages[ERR_UNKNOWNMODE] = "{mode} :is unknown mode char to me for {channel}";
	messages[ERR_UMODEUNKNOWNFLAG] = ":Unknown MODE flag";
	messages[ERR_INVALIDMODEPARAM] = "{channel} {mode} {value} :Invalid mode parameter";
	messages[ERR_NOMOTD] = ":MOTD File is missing";
	return messages;
}

std::map<IrcCode, RplFunctionPointer> Response::initialize_rpl_functions()
{
	std::map<IrcCode, RplFunctionPointer> functions;

	functions[RPL_WELCOME] = &Response::rpl_welcome;
	functions[RPL_YOURHOST] = &Response::rpl_yourhost;
	functions[RPL_CREATED] = &Response::rpl_created;
	functions[RPL_MYINFO] = &Response::rpl_myinfo;
	functions[RPL_ISUPPORT] = &Response::rpl_isupport;
	functions[RPL_LUSERCLIENT] = &Response::rpl_luserclient;
	functions[RPL_LUSEROP] = &Response::rpl_luserop;
	functions[RPL_LUSERUNKNOWN] = &Response::rpl_luserunknown;
	functions[RPL_LUSERCHANNELS] = &Response::rpl_luserchannels;
	functions[RPL_LUSERME] = &Response::rpl_luserme;
	functions[RPL_LOCALUSERS] = &Response::rpl_localusers;
	functions[RPL_GLOBALUSERS] = &Response::rpl_globalusers;
	functions[RPL_CHANNELMODEIS] = &Response::rpl_channelmodeis;
	functions[RPL_CREATIONTIME] = &Response::rpl_creationtime;
	functions[RPL_NOTOPIC] = &Response::rpl_notopic;
	functions[RPL_TOPIC] = &Response::rpl_topic;
	functions[RPL_TOPICWHOTIME] = &Response::rpl_topicwhotime;
	functions[RPL_INVITING] = &Response::rpl_inviting;
	functions[RPL_NAMREPLY] = &Response::rpl_namreply;
	functions[RPL_ENDOFNAMES] = &Response::rpl_endofnames;
	functions[RPL_MOTD] = &Response::rpl_motd;
	functions[RPL_MOTDSTART] = &Response::rpl_motdstart;
	functions[RPL_ENDOFMOTD] = &Response::rpl_endofmotd;
	return functions;
}

void Response::set_targets(const User* user, const Channel* channel) //  still needed ?
{
	user_ = const_cast<User*>(user);
	channel_ = const_cast<Channel*>(channel);
}

void Response::set_user(const User* user)
{
	user_ = const_cast<User*>(user);
}

void Response::set_channel(const Channel* channel)
{
	channel_ = const_cast<Channel*>(channel);
}

void Response::set_command(const std::string& command)
{
	command_ = command;
}

void Response::add_param(const std::string& key, const std::string& value)
{
	params_[key] = value;
}

std::map<std::string, std::string>& Response::get_params()
{
	return params_;
}

void Response::params_clear()
{
	if (!params_.empty())
	{
		params_.clear();
	}
}

std::string Response::generate_message(IrcCode code)
{
	std::string message = irc_messages_[code];
    std::map<std::string, std::string>::iterator it;

    for (it = params_.begin(); it != params_.end(); ++it)
    {
        std::string placeholder = "{" + it->first + "}";
        size_t pos = message.find(placeholder);
        while (pos != std::string::npos)
        {
            message.replace(pos, placeholder.size(), it->second);
            pos = message.find(placeholder);
        }
    }
    return message;
}

void Response::error_reply(IrcCode code)
{
	std::string message = Response::generate_message(code);
	std::ostringstream oss;

	oss << std::fixed << std::setprecision(3) << static_cast<int>(code);
	std::string code_str = oss.str();
	user_->send_message(":" + user_->get_server_name() + " " + code_str + \
		" " + user_->get_prefix() + " " + message + "\r\n");
	params_.clear(); // is it here?
	Logger::Log(ERROR, message);
}

std::string Response::rpl_welcome(IrcCode code)
{
	Response::add_param("server_nick", user_->get_server()->get_description());
	Response::add_param("client_prefix", user_->get_prefix());
	return Response::generate_message(code);
}

std::string Response::rpl_yourhost(IrcCode code)
{
	Response::add_param("server_name", user_->get_server_name());
	Response::add_param("server_version", user_->get_server()->get_version());
	return Response::generate_message(code);
}

std::string Response::rpl_created(IrcCode code)
{
	Response::add_param("server_date", to_string_(user_->get_server()->get_start_time()));
	return Response::generate_message(code);
}

std::string Response::rpl_myinfo(IrcCode code)
{
	Response::add_param("server_name", user_->get_server_name());
	Response::add_param("server_version", user_->get_server()->get_version());
	Response::add_param("channel_modes", user_->get_server()->get_available_channel_modes());
	return Response::generate_message(code);
}

std::string Response::rpl_isupport(IrcCode code)
{
	std::string tokens;

	tokens += "CHANMODES=,ko,l,it";
	tokens += " NICKLEN=10";
	// tokens += " TOPICLEN=120"; // implement !
	Response::add_param("tokens", tokens);
	return Response::generate_message(code);
}

std::string Response::rpl_luserclient(IrcCode code)
{
	Response::add_param("n_clients", to_string_(user_->get_server()->get_users().size()));
	Response::add_param("n_servers", to_string_(NR_OF_SERVERS));
	return Response::generate_message(code);
}

std::string Response::rpl_luserop(IrcCode code)
{
	Response::add_param("n_operators", "0");
	return Response::generate_message(code);
}

std::string Response::rpl_luserunknown(IrcCode code)
{
	int count = 0;
	std::vector<User*> users = user_->get_server()->get_users();

	for (size_t i = 0; i < users.size(); ++i)
	{
		if (users[i]->get_flags() & REGISTERED)
		{
			continue ;
		}
		++count;
	}
	Response::add_param("n_unknown", to_string_(count));
	return Response::generate_message(code);
}

std::string Response::rpl_luserchannels(IrcCode code)
{
	Response::add_param("n_channels", to_string_(user_->get_server()->get_channels().size()));
	return Response::generate_message(code);
}

std::string Response::rpl_luserme(IrcCode code)
{
	Response::add_param("n_clients", to_string_(user_->get_server()->get_users().size()));
	Response::add_param("n_servers", to_string_(NR_OF_SERVERS));
	return Response::generate_message(code);
}

std::string Response::rpl_localusers(IrcCode code)
{
	Response::add_param("n_local", to_string_(user_->get_server()->get_users().size()));
	Response::add_param("n_local_max", to_string_(user_->get_server()->get_max_local_users()));
	return Response::generate_message(code);
}

std::string Response::rpl_globalusers(IrcCode code)
{
	Response::add_param("n_global", to_string_(user_->get_server()->get_users().size()));
	Response::add_param("n_global_max", to_string_(user_->get_server()->get_max_local_users()));
	return Response::generate_message(code);
}

std::string Response::rpl_channelmodeis(IrcCode code)
{
	std::string activated_modes = "+";
	std::string mode_params;
	
	if (channel_->get_flags() & INVITEONLY)
	{
		activated_modes += "i";
	}
	if (channel_->get_flags() & TOPICMODE)
	{
		activated_modes += "t";
	}
	if (channel_->get_flags() & CHANNELKEY)
	{
		activated_modes += "k";
		mode_params += channel_->get_password(); // key !
	}
	if (channel_->get_flags() & USERLIMIT)
	{
		activated_modes += "l";
		mode_params += to_string_(channel_->get_user_limit());
	}
	Response::add_param("channel", channel_->get_name()); // redundant ?
	Response::add_param("activated_modes", activated_modes);
	return Response::generate_message(code);
}

std::string Response::rpl_creationtime(IrcCode code)
{
	Response::add_param("channel", channel_->get_name());
	Response::add_param("ch_timestamp", to_string_(channel_->get_start_time()));
	return Response::generate_message(code);
}

std::string Response::rpl_notopic(IrcCode code)
{
	Response::add_param("channel", channel_->get_name());
	return Response::generate_message(code);
}

std::string Response::rpl_topic(IrcCode code)
{
	Response::add_param("channel", channel_->get_name());
	Response::add_param("topic", channel_->get_topic());
	return Response::generate_message(code);
}

std::string Response::rpl_topicwhotime(IrcCode code)
{
	Response::add_param("channel", channel_->get_name());
	Response::add_param("nickname", user_->get_nickname());
	Response::add_param("t_timestamp", to_string_(channel_->get_topic_time()));
	Response::add_param("topic", channel_->get_topic());
	return Response::generate_message(code);
}

std::string Response::rpl_inviting(IrcCode code)
{
	Response::add_param("nickname", user_->get_nickname());
	Response::add_param("channel", channel_->get_name());
	Response::add_param("target_nickname", params_["target_nickname"]); // !
	return Response::generate_message(code);
}

std::string Response::rpl_namreply(IrcCode code)
{
	std::string nicknames;
	std::vector<const User*> users = channel_->get_users();
	std::vector<const User*> ops = channel_->get_operators();

	for (size_t i = 0; i < users.size(); ++i)
	{
		if (std::find(ops.begin(), ops.end(), users[i]) != ops.end())
		{
			nicknames += "@";
		}
		nicknames += users[i]->get_nickname();
		if (i < users.size() - 1)
		{
			nicknames += " ";
		}
	}
	Response::add_param("channel", channel_->get_name()); // redundant ?
	Response::add_param("nicknames", nicknames);
	return Response::generate_message(code);
}

std::string Response::rpl_endofnames(IrcCode code)
{
	if (channel_)
	{
		Response::add_param("channel", channel_->get_name()); // fails when for an invalid channel !
	}
	return Response::generate_message(code);
}

std::string Response::rpl_motd(IrcCode code)
{
	return Response::generate_message(code);
}

std::string Response::rpl_motdstart(IrcCode code)
{
	Response::add_param("server_name", user_->get_server_name());
	return Response::generate_message(code);
}

std::string Response::rpl_endofmotd(IrcCode code)
{
	return Response::generate_message(code);
}

void Response::reply(IrcCode code)
{
	std::string message;
	std::ostringstream oss;
	
	if (rpl_functions_.find(code) != rpl_functions_.end())
	{
		message = rpl_functions_[code](code);
		Logger::Log(INFO, message);
	}
	else
	{
		message = Response::generate_message(code);
		Logger::Log(ERROR, message);
	}
	oss << std::fixed << std::setprecision(3) << static_cast<int>(code);
	user_->send_message(":" + user_->get_server_name() + " " + oss.str() + \
		" " + user_->get_prefix() + " " + message + "\r\n");
	
}

void Response::reset()
{
	user_ = NULL;
	channel_ = NULL;
	params_.clear();
}
