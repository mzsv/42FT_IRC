/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:49:09 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/27 16:41:33 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(const std::string& message)
{
	message_ = message;
	std::string trimmed = std::string(message.begin(), message.end() - 1);
	std::queue<std::string> q = split2queue(trimmed, ' ', false);
	if (q.size() > 0 && q.front()[0] == ':')
	{
		prefix_ = std::string(q.front().begin() + 1, q.front().end());
	}
	if (q.size())
	{
		command_ = q.front();
	}
	q.pop();
	while (q.size())
	{
		if (q.front()[0] == ':')
		{
			std::string s(q.front().begin() + 1, q.front().end());
			q.pop(); // keeping the ":" so we know it was a trailing argument, needed for certain actions?
			while (q.size())
			{
				s.append(" ");
				s.append(q.front());
				q.pop();
			}
			arguments_.push_back(s);
			trailing_ = s;
		}
		else
		{
			arguments_.push_back(q.front());
			q.pop();
		}
	}

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

