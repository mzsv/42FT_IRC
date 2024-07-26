/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/26 22:45:16 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::running_ = true;

Server::Server(int port, const std::string& password) :
	port_(port), password_(password), timeout_(1),
	max_inactive_time_(120), max_response_time_(60),
	description("42 IRC Server"), version("1.0"),
	available_channel_modes("itkol"), max_local_users_(0)
{
	time(&start_time_);
	isupport_params_["CASEMAPPING"] = "ascii";
	isupport_params_["CHANLIMIT"] = "#:";
	isupport_params_["CHANMODES"] = ",ko,l,it";
	isupport_params_["CHANNELLEN"] = "42";
	isupport_params_["ELIST"] = "CTU";
	isupport_params_["HOSTLEN"] = "64";
	isupport_params_["KICKLEN"] = "307";
	isupport_params_["MODES"] = "42";
	isupport_params_["NICKLEN"] = "9";
	isupport_params_["PREFIX"] = "(o)@";
	isupport_params_["STATUSMSG"] = "@";
	isupport_params_["TARGMAX"] = "JOIN:,PART:,NAMES:,KICK:1,INVITE:,WHO:1,WHOIS:1,PRIVMSG:1";
	isupport_params_["TOPICLEN"] = "307";
	isupport_params_["USERLEN"] = "9";
}

Server::~Server()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		delete users_[i];
	}
	std::map<std::string, Channel*>::const_iterator it = channels_.begin();
	for (; it != channels_.end(); ++it)
	{
		delete it->second;
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
 * Funcs
*/
void Server::create_socket()
{
	socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_ == -1)
	{
		Logger::Log(FATAL, "Server::Failed to create socket");
		stop();
	}
}

void Server::bind_socket()
{
	int idk = 1;
	if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &idk, sizeof(int)) < 0)
	{
		Logger::Log(FATAL, "Server::Setsocketopt failed");
		return stop();
	}
	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_addr.s_addr = INADDR_ANY;
	sockaddr_.sin_port = htons(port_);
	if (bind(socket_fd_, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_)) < 0)
	{
		Logger::Log(FATAL, "Server::Failure to bind to port");
		stop();
	}
}

void Server::listen_socket()
{
	if (listen(socket_fd_, 128) < 0)
	{
		Logger::Log(FATAL, "Server::Failed to listen on socket");
		return stop();
	}
	fcntl(socket_fd_, F_SETFL, O_NONBLOCK);
	name_ = inet_ntoa(sockaddr_.sin_addr);

	Logger::Log(INFO, "Server running on " + std::string(inet_ntoa(sockaddr_.sin_addr)) + 
		":" + to_string_(ntohs(sockaddr_.sin_port)));
}

void Server::add_listener()
{
	struct pollfd pfd;
	pfd.fd = socket_fd_;
	pfd.events = POLLIN;
	pfd.revents = 0;
	users_fds_.push_back(pfd);
}

void Server::delete_broken_connection()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (!(users_[i]->get_flags() & BREAK))
		{
			continue;
		}
		if (users_[i]->get_flags() & TIMEOUT)
		{
			Response::add_param("reason", ":Ping timeout: " + to_string_(max_response_time_) + " seconds");
		}
		else if (users_[i]->get_flags() & UNPLUGGED)
		{
			Response::add_param("reason", ":Unplugged");
		}
		Response::cmd_reply(CMD_ERROR, NULL, *users_[i]);
		std::map<std::string, Channel*>::iterator begin = channels_.begin();
		std::map<std::string, Channel*>::iterator end = channels_.end();
		for (; begin != end; ++begin)
		{
			if (begin->second->contains_nickname(users_[i]->get_nickname()))
			{
				Response::cmd_reply(CMD_QUIT, *users_[i], *begin->second, false);
				begin->second->disconnect(*(users_[i]));
			}
		}
		close(users_[i]->get_socket_fd());
		Logger::Log(INFO, users_[i]->get_prefix() + " disconnected.");
		delete users_[i];
		users_.erase(users_.begin() + i);
		users_fds_.erase(users_fds_.begin() + i);
		--i;
	}
}

void Server::delete_empty_channels()
{
	std::map<std::string, Channel*>::iterator it = channels_.begin();
	
	while (it != channels_.end())
	{
		if (it->second->is_empty())
		{
			delete it->second;
			channels_.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void Server::check_connection()
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_flags() & REGISTERED)
		{
			if (difftime(time(0), users_[i]->get_time_of_last_action()) > max_inactive_time_)
			{
				Response::add_param("server", name_);
				Response::add_param("ping_token", name_);
				Response::cmd_reply(CMD_PING, NULL, *users_[i]);
				users_[i]->set_time_after_pinging();
				users_[i]->set_time_of_last_action();
				users_[i]->set_flag(PINGING);
			}
			if ((users_[i]->get_flags() & PINGING) && \
				difftime(time(0), users_[i]->get_time_after_pinging()) > max_response_time_)
			{
				users_[i]->set_flag(BREAK);
				users_[i]->set_flag(TIMEOUT);
			}
		}
	}
}

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
			user.set_time_of_registrations();
			Response::num_reply(RPL_WELCOME);
			Response::num_reply(RPL_YOURHOST);
			Response::num_reply(RPL_CREATED);
			Response::num_reply(RPL_MYINFO);
			Response::num_reply(RPL_ISUPPORT);
			Executor executor(this);
			executor.execute(Message("LUSERS\n"), user);
			executor.execute(Message("MOTD\n"), user);
			Logger::Log(INFO, "New client registered: " + user.get_prefix());
		}
	}
	else
	{
		Response::add_param("reason", ":Correct password required");
		return DISCONNECT;
	}
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
	char* host = inet_ntoa(sockaddr_.sin_addr);
	struct pollfd pfd;
	pfd.fd = connection;
	pfd.events = POLLIN;
	pfd.revents = 0;
	users_fds_.push_back(pfd);
	users_.push_back(new User(connection, host, this, name_));
	max_local_users_ = std::max(max_local_users_, users_.size());
}

void Server::process_message()
{
	int p = poll(users_fds_.data(), users_fds_.size(), timeout_);
	if (p <= 0)
	{
		return ;
	}
	for (size_t i = 0; i < users_fds_.size(); ++i)
	{
		if (users_fds_[i].revents & POLLIN)
		{
			if (users_fds_[i].fd == socket_fd_)
			{
				get_connection();
			}
			else
			{
				size_t j = i - 1;
				Response::set_user(users_[j]);
				if (users_[j]->read_message() == DISCONNECT)
				{
					users_[j]->set_flag(BREAK);
				}
				else if (handle_message(*(users_[j])) == DISCONNECT)
				{
					users_[j]->set_flag(BREAK);
				}
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
		if (!(user.get_flags() & REGISTERED) && message.get_command() != "QUIT" && message.get_command() != "PASS" \
			&& message.get_command() != "USER" && message.get_command() != "NICK" \
			&& message.get_command() != "CAP")
		{
			Response::num_reply(ERR_NOTREGISTERED);
		}
		else
		{			
			Executor executor(this);
			int response = executor.execute(message, user);
			if (response == DISCONNECT)
			{
				return DISCONNECT;
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
				break ;
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

int Server::join_channel(const std::string& name, const std::string& key, User& user)
{
	if (channels_.find(name) != channels_.end())
	{
		if ((channels_[name]->get_flags() & USERLIMIT) && channels_[name]->get_users().size() >= channels_[name]->get_user_limit())
		{
			Response::num_reply(ERR_CHANNELISFULL);
		}
		else if ((channels_[name]->get_flags() & INVITEONLY) && !channels_[name]->is_invited(&user))
		{
			Response::num_reply(ERR_INVITEONLYCHAN);
		}
		else if ((channels_[name]->get_flags() & CHANNELKEY) && !channels_[name]->is_invited(&user) \
			&& channels_[name]->get_password() != key)
		{
			Response::num_reply(ERR_BADCHANNELKEY);
		}
		else if (!channels_[name]->contains_nickname(user.get_nickname()))
		{
			Response::set_channel(channels_[name]);
			user.add_channel(*channels_[name]);
			channels_[name]->remove_invite(&user);
			return channels_[name]->add_user(user);
		}
	}
	else
	{
		channels_[name] = new Channel(name, "", user);
		user.add_channel(*channels_[name]);
		Response::set_channel(channels_[name]);
		Response::cmd_reply(CMD_JOIN, user, *channels_[name]);
		if (channels_[name]->get_topic().size())
		{
			Response::num_reply(RPL_TOPIC);
			Response::num_reply(RPL_TOPICWHOTIME);
		}
		Response::num_reply(RPL_NAMREPLY);
		Response::num_reply(RPL_ENDOFNAMES);
		return 0;
	}
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

void Server::leave_channel(const std::string& name, User& user)
{
	if (channels_.find(name) != channels_.end())
	{
		if (channels_[name]->contains_nickname(user.get_nickname()))
		{
			channels_[name]->disconnect(user);
			user.remove_channel(*channels_[name]);
		}
	}
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

const std::string Server::get_channel_topic(const std::string& channel_name) const
{
	std::string res;

	if (channels_.find(channel_name) != channels_.end())
	{
		return channels_.at(channel_name)->get_topic();
	}
	return res;
}

User* Server::get_user(const std::string& nickname) const
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		if (users_[i]->get_nickname() == nickname)
		{
			return users_[i];
		}
	}
	return 0;
}

const time_t& Server::get_start_time() const
{
	return start_time_;
}

const std::string& Server::get_description() const
{
	return description;
}

const std::string& Server::get_version() const
{
	return version;
}

const std::string& Server::get_available_channel_modes() const
{
	return available_channel_modes;
}

const std::vector<User*>& Server::get_users() const
{
	return users_;
}

const size_t& Server::get_max_local_users() const
{
	return max_local_users_;
}

const std::map<std::string, std::string>& Server::get_isupport_params() const
{
	return isupport_params_;
}

const std::string& Server::get_isupport_param(const std::string& key) const
{
	if (isupport_params_.find(key) == isupport_params_.end())
	{
		return key;
	}
	return isupport_params_.at(key);
}

void Server::stop()
{
	running_ = false;
	Logger::Log(INFO, "Server shutting down");
}

const bool& Server::is_running() const
{
	return running_;
}
