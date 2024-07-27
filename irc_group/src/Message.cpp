/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:49:09 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/27 13:54:47 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(const std::string& message)
{
	message_ = message;
	trailing_flag_ = false;
	std::string trimmed = std::string(message.begin(), message.end() - 1);
	std::queue<std::string> q = split2queue(trimmed, ' ', false);
	if (q.size() > 0 && q.front()[0] == ':')
	{
		prefix_ = std::string(q.front().begin() + 1, q.front().end());
		q.pop();
	}
	if (q.size())
	{
		command_ = q.front();
		q.pop();
	}
	while (q.size())
	{
		if (q.front()[0] == ':')
		{
			std::string s(q.front().begin() + 1, q.front().end());
			q.pop();
			while (q.size())
			{
				s.append(" ");
				s.append(q.front());
				q.pop();
			}
			arguments_.push_back(s);
			trailing_flag_ = true;
		}
		else
		{
			arguments_.push_back(q.front());
			q.pop();
		}
	}
}

Message::~Message()
{
}

const std::string& Message::get_message() const
{
	return message_;
}

const std::string& Message::get_command() const
{
	return command_;
}

const std::string& Message::get_prefix() const
{
	return prefix_;
}

const std::vector<std::string>& Message::get_arguments() const
{
	return arguments_;
}

bool Message::get_trailing_flag() const
{
	return trailing_flag_;
}

const std::string Message::get_trailing() const
{
	return trailing_flag_ ? arguments_[arguments_.size() - 1] : "";
}
