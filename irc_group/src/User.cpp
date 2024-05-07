/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:45:02 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/07 18:36:01 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int socket_fd, const std::string& host, std::string& server_name) :
	socket_fd_(socket_fd),
	host_(host),
	server_name_(server_name),
	flags_(RECEIVENOTICE),
	time_of_registrations_(time(0))
{
}

User::~User()
{
}

/**
 * Getters
*/
int User::get_socket_fd() const
{
	return socket_fd_;
}

const std::string& User::get_host() const
{
	return host_;
}

const std::string& User::get_server_name() const
{
	return server_name_;
}

const std::string& User::get_password() const
{
	return password_;
}

const std::string& User::get_nickname() const
{
	return nickname_;
}

const std::string& User::get_username() const
{
	return username_;
}

unsigned char User::get_flags() const
{
	return flags_;
}

time_t User::get_time_of_registrations() const
{
	return time_of_registrations_;
}

time_t User::get_time_of_last_action() const
{
	return time_of_last_action_;
}

time_t User::get_time_after_pinging() const
{
	return time_after_pinging_;
}

/**
 * Setters
*/
void User::set_password(const std::string& password)
{
	password_ = password;
}

void User::set_nickname(const std::string& nickname)
{
	nickname_ = nickname;
}

void User::set_username(const std::string& username)
{
	username_ = username;
}

void User::set_flag(unsigned char flag)
{
	flags_ |= flag;
}

void User::set_time_of_registrations()
{
	time_of_registrations_ = time(0);
}

void User::set_time_after_pinging()
{
	time_after_pinging_ = time(0);
}

void User::set_time_of_last_action()
{
	time_of_last_action_ = time(0);
}

const std::vector<const Channel*>& User::get_channels() const
{
	return channels_;
}

/**
 * Funcs
*/
const std::queue<std::string>& User::get_message() const
{
	return messages_;
}

void User::pop_message()
{
	if (messages_.size())
	{
		messages_.pop();
	}
}

void User::send_message(const std::string& message) const
{
	if (message.size() > 0)
	{
		send(socket_fd_, message.c_str(), message.size(), IRC_NOSIGNAL);
	}
}

int User::read_message()
{
	char bf[100];
	std::string msg;
	if (messages_.size() > 0)
	{
		msg = messages_.front();
	}
	int bytes;
	while ((bytes = recv(socket_fd_, bf, 99, 0)) > 0)
	{
		bf[bytes] = 0;
		msg += bf;
		memset(bf, 0, sizeof(bf));
		if (msg.find('\n') != std::string::npos)
		{
			break;
		}
	}
	if (msg.size() > 512)
	{
		msg = msg.substr(0, 510) + "\r\n";
	}
	if (bytes == 0)
	{
		return DISCONNECT;
	}
	while (msg.find("\r\n") != std::string::npos)
	{
		msg.replace(msg.find("\r\n"), 2, "\n");
	}
	if (msg.size() > 1)
	{
		messages_ = split2queue(msg, '\n', true);
	}
	return 0;
}

void User::reset_flag(unsigned char flag)
{
	flags_ &= ~flag;
}

std::string User::get_prefix() const
{
	return std::string(nickname_ + "!" + username_ + "@" + host_);
}

//!
std::string User::get_quit_message() const
{
	return "Quit message template";
}
