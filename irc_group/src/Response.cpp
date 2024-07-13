/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:55:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/13 20:08:48 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<IrcCode, std::string> Response::irc_messages_ = Response::initialize_irc_messages();

std::map<std::string, std::string> Response::params_;

std::map<IrcCode, std::string> Response::initialize_irc_messages()
{
	std::map<IrcCode, std::string> messages;

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

	user.send_message(user.get_prefix() + " " + message + "\r\n");
	params_.clear(); // is it here?
	Logger::Log(ERROR, message);
}

void Response::error(const User& user, int error, const std::string& arg1, const std::string& arg2)
{
	(void)arg1;
	(void)arg2;
	std::string message = ":" + user.get_server_name() + " ";
	// message += error
	message += " " + user.get_nickname();
	switch (error)
	{
	case ERR_NOTREGISTERED:
		/* code */
		break;

	default:
		break;
	}
}

void Response::reply(const User& user, int reply, const std::string& arg1, const std::string& arg2)
{
	(void)arg1;
	(void)arg2;
	std::string message = ":" + user.get_server_name() + " ";
	// message += reply
	message += " " + user.get_nickname();
	switch (reply)
	{
	case RPL_TOPIC:
		/* code */
		break;

	default:
		break;
	}
}
