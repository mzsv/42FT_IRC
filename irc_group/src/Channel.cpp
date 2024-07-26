/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:32:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/26 18:35:17 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name, const std::string& password, const User& creator) :
	name_(name), password_(password), user_limit_(0),
	flags_(NOMSGOUT | TOPICMODE), topic_time_(0)
{
	users_.push_back(&creator);
	operators_.push_back(&creator);
	start_time_ = time(NULL);
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

const time_t& Channel::get_topic_time() const
{
	return topic_time_;
}

const unsigned short& Channel::get_user_limit() const
{
	return user_limit_;
}

const User* Channel::get_user(const std::string& nickname) const
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_nickname() == nickname)
		{
			return users_[i];
		}
	}
	return NULL;
}

const time_t& Channel::get_start_time() const
{
	return start_time_;
}

/**
 * Setters
*/
void Channel::set_topic(const User& user, std::string topic)
{
	if ((flags_ & TOPICMODE) && !is_operator(user))
	{
		Response::num_reply(ERR_CHANOPRIVSNEEDED);
	}
	else
	{
		topic_ = topic;
	}
}

void Channel::set_password(const User& user, std::string password)
{
	if (!is_operator(user))
	{
		Response::num_reply(ERR_CHANOPRIVSNEEDED);
	}
	else
	{
		password_ = password;
	}
}

void Channel::set_topic_time()
{
	topic_time_ = time(NULL);
}

/**
 * Funcs
*/
bool Channel::contains_nickname(const std::string& nickname) const
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_nickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

bool Channel::is_empty()
{
	return users_.empty();
}

void Channel::disconnect(const User& user)
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i] == &user)
		{
			users_.erase(users_.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < operators_.size(); ++i)
	{
		if (operators_[i] == &user)
		{
			operators_.erase(operators_.begin() + i);
			break ;
		}
	}
}

bool Channel::is_operator(const User& user) const
{
	for (size_t i = 0; i < operators_.size(); ++i)
	{
		if (operators_[i]->get_prefix() == user.get_prefix())
		{
			return true;
		}
	}
	return false;
}

bool Channel::is_operator(const std::string& nickname) const
{
	for (size_t i = 0; i < operators_.size(); ++i)
	{
		if (operators_[i]->get_nickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

void Channel::send_message(const std::string& message, const User& user, bool include_user) const
{
	std::vector<const User*>::const_iterator begin = users_.begin();
	std::vector<const User*>::const_iterator end = users_.end();

	for (; begin != end; ++begin)
	{
		if (include_user || *begin != &user)
		{
			(*begin)->send_message(message);
		}
	}
}

int Channel::add_user(const User& user)
{
	if (user_limit_ == 0 || users_.size() < user_limit_)
	{
		users_.push_back(&user);
		Response::cmd_reply(CMD_JOIN, user, *this);
		if (topic_.size())
		{
			Response::num_reply(RPL_TOPIC);
			Response::num_reply(RPL_TOPICWHOTIME);
		}
		Response::num_reply(RPL_NAMREPLY);
		Response::num_reply(RPL_ENDOFNAMES);
	}
	else
	{
		Response::num_reply(ERR_CHANNELISFULL);
	}
	return 0;
}

const std::vector<const User*>& Channel::get_users() const
{
	return users_;
}

void Channel::set_flag(unsigned char flag)
{
	flags_ |= flag;
}

void Channel::reset_flag(unsigned char flag)
{
	flags_ &= ~flag;
}

void Channel::set_user_limit(unsigned short limit)
{
	user_limit_ = limit;
}

void Channel::add_operator(std::string nickname)
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_nickname() == nickname)
		{
			operators_.push_back(users_[i]);
			break ;
		}
	}
}

void Channel::remove_operator(std::string nickname)
{
	for (size_t i = 0; i < operators_.size(); ++i)
	{
		if (operators_[i]->get_nickname() == nickname)
		{
			operators_.erase(operators_.begin() + i);
			break ;
		}
	}
}

const std::vector<const User*>& Channel::get_operators() const
{
	return operators_;
}

bool Channel::is_invited(const User* user) const
{
	return invites_.find(user) != invites_.end();
}

void Channel::add_invite(const User* user)
{
	invites_.insert(user);
}

void Channel::remove_invite(const User* user)
{
	invites_.erase(user);
}

void Channel::clear_invites()
{
	invites_.clear();
}
