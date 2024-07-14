/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:55:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/14 21:21:43 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<IrcCode, std::string> Response::irc_messages_ = Response::initialize_irc_messages();

std::map<std::string, std::string> Response::params_;

std::map<IrcCode, RplFunctionPointer> Response::rpl_functions_ = Response::initialize_rpl_functions();

std::map<IrcCode, std::string> Response::initialize_irc_messages()
{
	std::map<IrcCode, std::string> messages;

	messages[RPL_WELCOME] = ":Welcome to the ft_IRC Network, {client_prefix}"; // client_prefix = nickname!username@hostname
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
	messages[RPL_INVITING] = "{nickname} {channel} :Inviting {nickname} to {channel}";
	messages[RPL_NAMREPLY] = "{channel} :{nicknames}"; // NOT REQUIRED ! nicknames = "@nickname1 +nickname2 nickname3" @ for op, + for voice
	messages[RPL_ENDOFNAMES] = "{channel} :End of /NAMES list";
	messages[RPL_MOTD] = ":- {message_of_the_day}";
	messages[RPL_MOTDSTART] = ":- {server_name} Message of the Day - ";
	messages[RPL_ENDOFMOTD] = ":End of /MOTD command";
	/* - When responding to the MOTD message and the MOTD file
           is found, the file is displayed line by line, with
           each line no longer than 80 characters, using
           RPL_MOTD format replies.  These MUST be surrounded
           by a RPL_MOTDSTART (before the RPL_MOTDs) and an
           RPL_ENDOFMOTD (after). */
	messages[ERR_NOSUCHNICK] = "{nickname} :No such nick/channel";
	messages[ERR_NOSUCHCHANNEL] = "{channel} :No such channel";
	messages[ERR_CANNOTSENDTOCHAN] = "{channel} :Cannot send to channel";
	messages[ERR_NOORIGIN] = ":No origin specified";
	messages[ERR_NORECIPIENT] = ":No recipient given ({command})";
	messages[ERR_NOTEXTTOSEND] = ":No text to send";
	messages[ERR_UNKNOWNCOMMAND] = "{command} :Unknown command";
	messages[ERR_USERNOTINCHANNEL] = "{nickname} :is not on channel {channel}";
	messages[ERR_NOTONCHANNEL] = "{nickname} :is not on channel {channel}";
	messages[ERR_USERONCHANNEL] = "{nickname} :is already on channel {channel}";
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

void Response::error_reply(const User& user, IrcCode code)
{
	std::string message = Response::generate_message(code);
	std::ostringstream oss;

	oss << std::fixed << std::setprecision(3) << static_cast<int>(code);
	std::string code_str = oss.str();
	user.send_message(":" + user.get_server_name() + " " + code_str + \
		" " + user.get_prefix() + " " + message + "\r\n");
	params_.clear(); // is it here?
	Logger::Log(ERROR, message);
}

// void Response::error(const User& user, int error, const std::string& arg1, const std::string& arg2)
// {
// 	(void)arg1;
// 	(void)arg2;
// 	std::string message = ":" + user.get_server_name() + " ";
// 	// message += error
// 	message += " " + user.get_nickname();
// 	switch (error)
// 	{
// 	case ERR_NOTREGISTERED:
// 		/* code */
// 		break;

// 	default:
// 		break;
// 	}
// }

// void Response::reply(const User& user, int reply, const std::string& arg1, const std::string& arg2)
// {
// 	(void)arg1;
// 	(void)arg2;
// 	std::string message = ":" + user.get_server_name() + " ";
// 	// message += reply
// 	message += " " + user.get_nickname();
// 	switch (reply)
// 	{
// 	case RPL_TOPIC:
// 		/* code */
// 		break;

// 	default:
// 		break;
// 	}
// }


