/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:45:02 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/25 21:18:04 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int socket_fd, const std::string& host, const Server* server, std::string& server_name) :
	socket_fd_(socket_fd),
	host_(host),
	server_name_(server_name),
	server_(server),
	flags_(RECEIVENOTICE),
	time_of_registrations_(time(0))
{
}

User::~User()
{
	Logger::Log(INFO, "User destructor");
	close(socket_fd_);
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

const Server* User::get_server() const
{
	return server_;
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

const std::string& User::get_realname() const
{
	return realname_;
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
	// std::cout << "Setting nickname to: " << nickname << std::endl;
	nickname_ = nickname;
	// Logger::Log(DEBUG, "Setting nickname to: " + nickname);
}

void User::set_username(const std::string& username)
{
	// std::cout << "Setting username to: " << username << std::endl;
	username_ = username;
	// Logger::Log(DEBUG, "Setting username to: " + username);
}

void User::set_realname(const std::string& realname)
{
	realname_ = realname;
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
		if (send(socket_fd_, message.c_str(), message.size(), IRC_NOSIGNAL) < 0)
		{
			Logger::Log(ERROR, "Failed to send message");
		}
	}
	Logger::Log(INFO, "Sent message: " + message + " to " + nickname_);
}

int User::read_message()
{
	char bf[100];
	std::string msg;
	if (messages_.size() > 0)
	{
		msg = messages_.front();
	}
	// iterate over the messages and print content
	std::cout << "--Messages--: " << messages_.size() << std::endl;
	std::queue<std::string> temp = messages_;
	while (temp.size() > 0)
	{
		std::cout << "Message: " << temp.front() << std::endl;
		temp.pop();
	}
	int bytes;
	// Logger::Log(DEBUG, "Reading message");
	while ((bytes = recv(socket_fd_, bf, 99, 0)) > 0)
	{
		// Logger::Log(DEBUG, "Received bytes: " + to_string_(bytes));
		bf[bytes] = 0;
		// std::cout << "Received: " << bf << std::endl;
		msg += bf;
		memset(bf, 0, sizeof(bf));
		if (msg.find('\n') != std::string::npos)
		{
			break;
		}
	}
	if (bytes < 0)
	{
		Logger::Log(ERROR, "Failed to receive message");
		this->set_flag(UNPLUGGED);
		return DISCONNECT;
	}
	// Logger::Log(DEBUG, "recv returned: " + to_string_(bytes));
	if (msg.size() > 512)
	{
		msg = msg.substr(0, 510) + "\r\n";
	}
	if (bytes == 0)
	{
		Logger::Log(ERROR, "EOF: Client disconnected");
		this->set_flag(UNPLUGGED);
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
	// print message
	std::cout << "Message: " << msg << std::endl;
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

void User::add_channel(const Channel& channel)
{
	channels_.push_back(&channel);
}

void User::remove_channel(const Channel& channel)
{
	for (size_t i = 0; i < channels_.size(); ++i)
	{
		if (channels_[i] == &channel)
		{
			channels_.erase(channels_.begin() + i);
			break ;
		}
	}
}

time_t User::get_idle_time() const
{
	return time(0) - time_of_last_action_;
}
