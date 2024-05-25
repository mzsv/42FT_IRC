/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:32:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/25 20:35:13 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name, const std::string& password, const User& creator) :
	name_(name), password_(password), user_limit_(0), flags_(NOMSGOUT)
{
	users_.push_back(&creator);
	operators_.push_back(&creator);
}

Channel::~Channel()
{
}

/**
 * Getters
*/
const std::string& Channel::get_name() const
{
	return name_;
}

const std::string& Channel::get_password() const
{
	return password_;
}

unsigned char Channel::get_flags() const
{
	return flags_;
}

const std::string& Channel::get_topic() const
{
	return topic_;
}


/**
 * Setters
*/
void Channel::set_topic(const User& user, std::string topic)
{
	if ((flags_ & TOPICSET) && !is_operator(user))
	{
		Response::error(user, ERR_CHANOPRIVSNEEDED, name_);
	}
	else
	{
		topic_ = topic;
		send_message("TOPIC " + name_ + " :" + topic_ + "\n", user, true);
	}
}

void Channel::set_password(const User& user, std::string password) // review !
{
	if (!is_operator(user))
	{
		Response::error(user, ERR_CHANOPRIVSNEEDED, name_);
	}
	else
	{
		password_ = password;
	}
}

/**
 * Funcs
*/
// placeholder
bool Channel::contains_nickname(const std::string& nichname) const
{
	(void)nichname;
	return false;
}

// placeholder
bool Channel::is_empty()
{
	return false;
}

// placeholder
void Channel::disconnect(User& user)
{
	(void)user;
}

bool Channel::is_operator(const User& user) const
{
	for (size_t i = 0; i < operators_.size(); ++i)
	{
		if (speakers_[i]->get_prefix() == user.get_prefix())
		{
			return true;
		}
	}
	return false;
}

void Channel::send_message(const std::string& message, const User& user, bool include_user) const
{
	std::string to_send = ":" + user.get_prefix() + " " + message;
	std::vector<const User*>::const_iterator begin = users_.begin();
	std::vector<const User*>::const_iterator end = users_.end();
	for (; begin != end; ++begin)
	{
		if (include_user || *begin != &user)
		{
			(*begin)->send_message(to_send);
		}
	}
}

int Channel::add_user(const User& user)
{
	if (user_limit_ == 0 || users_.size() < user_limit_)
	{
		users_.push_back(&user);
		user.send_message(":" + user.get_prefix() + " JOIN " + name_ + "\r\n"); // \r\n or \n?
		if (flags_ & TOPICSET)
		{
			Response::reply(user, RPL_TOPIC, name_, topic_); // !
			Response::reply(user, RPL_TOPICWHOTIME, name_, user.get_nickname()); // !
		}
		Response::reply(user, RPL_NAMREPLY, name_, user.get_nickname()); // MUST include joining client
		Response::reply(user, RPL_ENDOFNAMES, name_);
		return 0; // not needed if messaging client and channel here !
	}
	else
	{
		Response::error(user, ERR_CHANNELISFULL, name_);
	}
	return -1;
}
