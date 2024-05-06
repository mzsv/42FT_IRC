/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/06 20:49:52 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logger.hpp"

/* ========================================================================== */

Server::Server(int port, const std::string& password) :
	port_(port), password_(password), timeout_(1)
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

	// print address and port
	std::cout << "Server is running on "
		<< inet_ntoa(sockaddr_.sin_addr) << ":"
		<< ntohs(sockaddr_.sin_port) << std::endl;
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
			if (time(0) - users_[i]->get_time_of_last_action() > max_inactive_time_)
			{
				users_[i]->send_message(":" + name_ + " PING :" + name_ + "\n");
				users_[i]->set_time_after_pinging();
				users_[i]->set_time_of_last_action();
				users_[i]->set_flag(PINGING);
			}
			if ((users_[i]->get_flags() & PINGING) && time(0) - users_[i]->get_time_after_pinging() > max_response_time_)
			{
				users_[i]->set_flag(BREAK);
			}
		}
	}
}

//! Refactor this method
int Server::check_connection(User& user)
{
	if (!user.get_nickname().size() || !user.get_username().size())
	{
		return 0;
	}
	if (!password_.size() || user.get_password() == password_)
	{
		if (!(user.get_flags() & REGISTERED))
		{
			user.set_flag(REGISTERED);
		}
	}
	else
	{
		return DISCONNECT;
	}
	return 0;
}

void Server::get_connection()
{
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
			&& message.get_command() != "USER" && message.get_command() != "NICK")
		{
			Response::error(user, ERR_NOTREGISTERED);
		}
		else
		{
			try
			{
				Executor executor(this);
				int response = executor.execute(message, user);
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
	user.set_time_of_last_action();
	return 0;
}

void Server::notify_users(User& user, const std::string& message)
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
