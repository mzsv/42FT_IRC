/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/28 16:50:34 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logger.hpp"

/* ========================================================================== */

Server::Server(int port, const std::string& password) :
	port_(port), password_(password), timeout_(1),
	max_inactive_time_(120), max_response_time_(60)
	// should the listener socket be setup here?
{
}

Server::~Server()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		close(users_[i]->get_socket_fd());
		delete users_[i];
	}
	std::map<std::string, Channel*>::const_iterator begin = channels_.begin();
	std::map<std::string, Channel*>::const_iterator end = channels_.end();
	for (; begin != end; ++begin)
	{
		delete (*begin).second;
	}
	close(socket_fd_);
}

/**
 * Getters
*/
int Server::get_socket_fd() const
{
	return socket_fd_;
}

const std::string& Server::get_name() const
{
	return name_;
}

const std::string& Server::get_password() const
{
	return password_;
}

const std::map<std::string, Channel*>& Server::get_channels() const
{
	return channels_;
}
/**
 * Setters
*/

/**
 * Funcs
*/
void Server::create_socket()
{
	socket_fd_ = socket(AF_INET, SOCK_STREAM, 0); // protocol = 0 leaves it up to the OS to choose the correct protocol
	if (socket_fd_ == -1)
	{
		Logger::Log(FATAL, "Failed to create socket.");
		exit(errno);
	}
}

void Server::bind_socket()
{
	int idk = 0;
	if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &idk, sizeof(int)) < 0)
	{
		Logger::Log(FATAL, "Setsocket failed.");
		exit(EXIT_FAILURE);
	}
	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_addr.s_addr = INADDR_ANY;
	sockaddr_.sin_port = htons(port_);
	if (bind(socket_fd_, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_)) < 0)
	{
		Logger::Log(FATAL, "Failure to bind to port.");
		exit(EXIT_FAILURE);
	}
}

void Server::listen_socket()
{
	if (listen(socket_fd_, 128) < 0)
	{
		Logger::Log(FATAL, "Failed to listen on socket.");
		exit(EXIT_FAILURE);
	}
	fcntl(socket_fd_, F_SETFL, O_NONBLOCK);
	name_ = inet_ntoa(sockaddr_.sin_addr);

	// print address and port
	std::cout << "Server is running on "
		<< inet_ntoa(sockaddr_.sin_addr) << ":"
		<< ntohs(sockaddr_.sin_port) << std::endl;
	// print times
	std::cout << "max_inactive_time_: " << max_inactive_time_ << std::endl;
	std::cout << "max_response_time_: " << max_response_time_ << std::endl;
	std::cout << "timeout_: " << timeout_ << std::endl;
}

/*
	Надо заменить итерацию по индексам на итерации по итераторам.
	Причем надо обратить внимание на инвалидацию итераторов при удалении элемента
*/
void Server::delete_broken_connection()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (!(users_[i]->get_flags() & BREAK))
		{
			continue;
		}
		notify_users(*(users_[i]), ":" + users_[i]->get_prefix() + " QUIT :" + users_[i]->get_quit_message() + "\n");
		close(users_[i]->get_socket_fd());
		std::map<std::string, Channel*>::iterator begin = channels_.begin();
		std::map<std::string, Channel*>::iterator end = channels_.end();
		for (; begin != end; ++begin)
		{
			if ((*begin).second->contains_nickname(users_[i]->get_nickname()))
			{
				(*begin).second->disconnect(*(users_[i]));
			}
		}
		delete users_[i];
		users_.erase(users_.begin() + i);
		users_fds_.erase(users_fds_.begin() + i);
		--i;
	}
}

/*
	Та же проблема с инвалидацией итераторов...
*/
void Server::delete_empty_channels()
{
	std::map<std::string, Channel*>::iterator begin = channels_.begin();
	std::map<std::string, Channel*>::iterator end = channels_.end();
	for (; begin != end; ++begin)
	{
		if ((*begin).second->is_empty())
		{
			delete (*begin).second;
			channels_.erase((*begin).first);
			begin = channels_.begin();
			continue;
		}
		++begin;
	}
}

void Server::check_connection()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_flags() & REGISTERED)
		{
			// if (time(0) - users_[i]->get_time_of_last_action() > max_inactive_time_)
			if (difftime(time(0), users_[i]->get_time_of_last_action()) > max_inactive_time_)
			{
				users_[i]->send_message(":" + name_ + " PING :" + name_ + "\r\n");
				users_[i]->set_time_after_pinging();
				users_[i]->set_time_of_last_action();
				users_[i]->set_flag(PINGING);
			}
			// if ((users_[i]->get_flags() & PINGING) && time(0) - users_[i]->get_time_after_pinging() > max_response_time_)
			if ((users_[i]->get_flags() & PINGING) && \
				difftime(time(0), users_[i]->get_time_after_pinging()) > max_response_time_)
			{
				users_[i]->set_flag(BREAK);
			}
		}
	}
}

//! Refactor this method
int Server::check_connection(User& user)
{
	// separate function for registration? ! no need to check this all the time
	if (!user.get_nickname().size() || !user.get_username().size())
	{
		return 0;
	}
	if (!password_.size() || user.get_password() == password_)
	{
		if (!(user.get_flags() & REGISTERED))
		{
			user.set_flag(REGISTERED); //  Registration
		}
	}
	else
	{
		return DISCONNECT;
	}
	// // check pinging time
	// if (user.get_flags() & PINGING && time(0) - user.get_time_after_pinging() > max_response_time_)
	// {
	// 	return DISCONNECT;
	// }
	// // check timeout
	// if (time(0) - user.get_time_of_last_action() > max_inactive_time_)
	// {
	// 	//maybe put this in a pinging function ? and reuse at ping_users
	// 	user.send_message(":" + name_ + " PING :" + name_ + "\n");
	// 	user.set_time_after_pinging();
	// 	user.set_time_of_last_action();
	// 	user.set_flag(PINGING);
	// }
	return 0;
}

void Server::get_connection()
{
	// doesn't poll check if there an incoming connection on the listening sock? is is more efficient?
	// yes! poll must be used prior to accept ! (check evaluation sheet)
	// ISSUE
	size_t addr_len = sizeof(sockaddr);
	int connection = accept(socket_fd_, (struct sockaddr*)&sockaddr_, (socklen_t*)&addr_len);
	if (connection < 0)
	{
		return;
	}
	char host[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(sockaddr_.sin_addr), host, INET_ADDRSTRLEN);
	struct pollfd pfd;
	pfd.fd = connection;
	pfd.events = POLLIN;
	pfd.revents = 0;
	users_fds_.push_back(pfd);
	users_.push_back(new User(connection, host, name_));

	// + missing protocol registration message; do I need it for this project?
	// I think we should, to confirm it responds to the correct protocol
}


void Server::process_message()
{
	int p = poll(users_fds_.data(), users_fds_.size(), timeout_);
	std::vector<int> to_erase;
	if (p == 0)
	{
		return;
	}
	for (size_t i = 0; i < users_fds_.size(); ++i)
	{
		if (users_fds_[i].revents & POLLIN)
		{
			if (users_[i]->read_message() == DISCONNECT)
			{
				users_[i]->set_flag(BREAK);
			}
			else if (handle_message(*(users_[i])) == DISCONNECT)
			{
				users_[i]->set_flag(BREAK);
			}
			users_fds_[i].revents = 0;
		}
	}
}

int Server::handle_message(User& user)
{
	while (user.get_message().size() > 0 &&
		user.get_message().front()[user.get_message().front().size() - 1] == '\n')
	{
		Message message(user.get_message().front());
		user.pop_message();
		Logger::Log(INFO, message.get_message());
		if (!(user.get_flags() & REGISTERED) && message.get_command() != "QUIT" && message.get_command() != "PASS" \
			&& message.get_command() != "USER" && message.get_command() != "NICK") // shorter way?
		{
			Response::error(user, ERR_NOTREGISTERED);
		}
		else
		{
			try
			{
				std::cout << "executing..." << std::endl;
				// print message members
				std::cout << "Message: " << message.get_message() << std::endl;
				std::cout << "Command: " << message.get_command() << std::endl;
				std::cout << "Prefix: " << message.get_prefix() << std::endl;
				for (size_t i = 0; i < message.get_arguments().size(); ++i)
				{
					std::cout << "Arg " << i << ": " << message.get_arguments()[i] << std::endl;
				}
				std::cout << "contains_trailing: " << message.contains_trailing() << std::endl;
				std::cout << "Trailing: " << message.get_trailing() << std::endl;
				
				Executor executor(this); // efficient to create an instance every time? static-ify? or make it a member?
				int response = executor.execute(message, user);
				std::cout << "response: " << response << std::endl;
				if (response == DISCONNECT)
				{
					return DISCONNECT;
				}
			}
			catch (const std::exception& e)
			{
				Response::error(user, ERR_UNKNOWNCOMMAND, message.get_command());
			}
		}
	}
	// print user info
	// std::cout << "nick: " << user.get_nickname() << " username: " << user.get_username() << " host: " << user.get_host() << std::endl;
	user.set_time_of_last_action();
	return 0;
}

void Server::notify_users(User& user, const std::string& message) // ?
{
	const std::vector<const Channel*> channels = user.get_channels();
	for (size_t i = 0; i < users_.size(); ++i)
	{
		for (size_t j = 0; j < channels.size(); ++j)
		{
			if (channels[j]->contains_nickname(users_[i]->get_nickname()))
			{
				users_[i]->send_message(message);
				break;
			}
		}
	}
}

bool Server::contains_nickname(const std::string& nickname) const
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

// void Server::ping_users() const // protocol conflict
// {
// 	for (size_t i = 0; i < users_.size(); ++i)
// 	{
// 		if (users_[i]->get_flags() & REGISTERED && !(users_[i]->get_flags() & PINGING))
// 		{
// 			users_[i]->send_message(":" + name_ + " PING :" + name_ + "\r\n");
// 		}
// 	}
// }

int Server::join_channel(const std::string& name, const std::string& key, const User& user) // review with modes
{
	if (channels_.find(name) != channels_.end())
	{
		if (channels_[name]->get_flags() & INVITEONLY && !channels_[name]->is_operator(user))
		{
			Response::error(user, ERR_INVITEONLYCHAN, name);
		}
		// assuming PRIVATE means mode k is on (key required for channel entry)
		else if (channels_[name]->get_flags() & CHANNELKEY && channels_[name]->get_password() != key)
		{
			Response::error(user, ERR_BADCHANNELKEY, name);
		}
		else if (!channels_[name]->contains_nickname(user.get_nickname()))
		{
			return channels_[name]->add_user(user);
		}
	}
	else
	{
		channels_[name] = new Channel(name, "", user);
		return 0;
	}
	// do we have a CHANLIMIT? should we?
	return -1;
}

bool Server::contains_channel(const std::string& name) const
{
	return channels_.find(name) != channels_.end();
}

bool Server::user_on_channel(const std::string& channel, const User& user) const
{
	if (channels_.find(channel) != channels_.end())
	{
		return channels_.at(channel)->contains_nickname(user.get_nickname());
	}
	return false;
}

bool Server::user_on_channel(const std::string& channel, const std::string& user) const
{
	if (channels_.find(channel) != channels_.end())
	{
		return channels_.at(channel)->contains_nickname(user);
	}
	return false;
}

bool Server::is_operator(const std::string& channel, const User& user) const
{
	if (channels_.find(channel) != channels_.end())
	{
		return channels_.at(channel)->is_operator(user);
	}
	return false;
}

void Server::leave_channel(const std::string& name, const User& user)
{
	if (channels_.find(name) != channels_.end())
	{
		if (channels_[name]->contains_nickname(user.get_nickname()))
		{
			channels_[name]->disconnect(user);
		}
	}
	// remove channel from User (really necessary to have a list of channels in User?)
}

void Server::list_users(const std::string& channel_name, const User& user) const
{
	// could also be implemented in executor, just need to add a get_channels() to Server to get list of channels?
	if (channel_name == "*")
	{
		for (size_t i = 0; i < channels_.size(); ++i)
		{
			user.send_message(":" + name_ + " 353 " + user.get_nickname() + " = " + channel_name + " :" + channels_.at(channel_name)->get_users() + "\n");
			Response::reply(user, RPL_ENDOFNAMES, channel_name);
		}
	}
	// RPL_NAMREPLY - implement in Response as the other replies?!
	user.send_message(":" + name_ + " 353 " + user.get_nickname() + " = " + channel_name + " :" + channels_.at(channel_name)->get_users() + "\n");
}

void Server::channel_broadcast(const std::string& channel_name, const User& user, const std::string& message) const
{
	if (channels_.find(channel_name) != channels_.end())
	{
		channels_.at(channel_name)->send_message(message, user, true);
	}
}

bool Server::check_channel_mode(const std::string& channel_name, const unsigned char& mode) const
{
	if (channels_.find(channel_name) != channels_.end())
	{
		return channels_.at(channel_name)->get_flags() & mode;
	}
	return false;
}

const std::string& Server::get_channel_topic(const std::string& channel_name) const
{
	if (channels_.find(channel_name) != channels_.end())
	{
		return channels_.at(channel_name)->get_topic();
	}
	return "";
}
