/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:28:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/24 21:59:26 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logger.hpp"

/* ========================================================================== */

bool Server::running_ = true;

Server::Server(int port, const std::string& password) :
	port_(port), password_(password), timeout_(1),
	max_inactive_time_(120), max_response_time_(60),
	description("42 IRC Server"), version("1.0"),
	available_channel_modes("itkol"), max_local_users_(0)
	// running_(true)
	// should the listener socket be setup here?
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
	Logger::Log(INFO, "Server destructor");
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
		// exit(errno);
		stop();
	}
}

void Server::bind_socket()
{
	int idk = 1;
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
		if (users_[i]->get_flags() & TIMEOUT)
		{
			Response::add_param("reason", ":Ping timeout: " + to_string_(max_response_time_) + " seconds");
		}
		else if (users_[i]->get_flags() & UNPLUGGED)
		{
			Response::add_param("reason", ":Unplugged");
		}
		// Response::add_param("reason", "Ping timeout: " + to_string_(max_response_time_) + " seconds");
		Response::reply(CMD_ERROR);
		std::map<std::string, Channel*>::iterator begin = channels_.begin();
		std::map<std::string, Channel*>::iterator end = channels_.end();
		for (; begin != end; ++begin)
		{
			if (begin->second->contains_nickname(users_[i]->get_nickname()))
			{
				Response::channel_reply(CMD_QUIT, *users_[i], *begin->second, false);
				begin->second->disconnect(*(users_[i]));
			}
		}
		close(users_[i]->get_socket_fd());
		Logger::Log(INFO, "User " + users_[i]->get_nickname() + " disconnected.");
		delete users_[i];
		users_.erase(users_.begin() + i); // ?
		users_fds_.erase(users_fds_.begin() + i);
		--i;
	}
}

/*
	Та же проблема с инвалидацией итераторов...
*/
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
	// print channels
	// for (it = channels_.begin(); it != channels_.end(); ++it)
	// {
	// 	std::cout << it->first << std::endl;
	
	// }
	// std::map<std::string, Channel*>::iterator begin = channels_.begin();
	// std::map<std::string, Channel*>::iterator end = channels_.end();
	// for (; begin != end; ++begin)
	// {
	// 	if ((*begin).second->is_empty())
	// 	{
	// 		delete (*begin).second;
	// 		channels_.erase((*begin).first);
	// 		begin = channels_.begin();
	// 		continue;
	// 	}
	// 	++begin;
	// }
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
				users_[i]->send_message(":" + name_ + " PING :" + name_ + "\r\n"); // ping message !
				users_[i]->set_time_after_pinging();
				users_[i]->set_time_of_last_action();
				users_[i]->set_flag(PINGING);
			}
			// if ((users_[i]->get_flags() & PINGING) && time(0) - users_[i]->get_time_after_pinging() > max_response_time_)
			if ((users_[i]->get_flags() & PINGING) && \
				difftime(time(0), users_[i]->get_time_after_pinging()) > max_response_time_)
			{
				users_[i]->set_flag(BREAK);
				users_[i]->set_flag(TIMEOUT);
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
			user.set_time_of_registrations(); // singular !
			Response::reply(RPL_WELCOME);
			Response::reply(RPL_YOURHOST);
			Response::reply(RPL_CREATED);
			Response::reply(RPL_MYINFO);
			Response::reply(RPL_ISUPPORT);
			Executor executor(this); // redundant !
			executor.execute(Message("LUSERS\n"), user);
			executor.execute(Message("MOTD\n"), user);
			Logger::Log(INFO, "User " + user.get_nickname() + " registered.");
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
	pfd.events |= POLLRDHUP;
	pfd.events |= POLLERR;
	pfd.events |= POLLHUP;
	pfd.events |= POLLNVAL;
	pfd.events |= POLLOUT;
	pfd.events |= POLLPRI;
	pfd.events |= POLLWRBAND;
	pfd.events |= POLLWRNORM;
	pfd.revents = 0;
	users_fds_.push_back(pfd);
	users_.push_back(new User(connection, host, this, name_));
	max_local_users_ = std::max(max_local_users_, users_.size());
	Logger::Log(INFO, "User connected from " + std::string(host) + "to " + name_);
	Logger::Log(DEBUG, "users_ size: " + to_string_(users_.size()));
	Logger::Log(DEBUG, "user->server_name: " + users_.back()->get_server_name());
	// + missing protocol registration message; do I need it for this project?
	// I think we should, to confirm it responds to the correct protocol
}

void print_poll_revents(const struct pollfd& pfd) // DEBUG !
{
	if (pfd.revents & POLLIN)
	{
		Logger::Log(DEBUG, "------------->POLLIN");
	}
	// if (pfd.revents & POLLOUT)
	// {
	// 	Logger::Log(DEBUG, "------------->POLLOUT");
	// }
	if (pfd.revents & POLLERR)
	{
		Logger::Log(DEBUG, "------------->POLLERR");
	}
	if (pfd.revents & POLLHUP)
	{
		Logger::Log(DEBUG, "------------->POLLHUP");
	}
	if (pfd.revents & POLLNVAL)
	{
		Logger::Log(DEBUG, "------------->POLLNVAL");
	}
	if (pfd.revents & POLLRDHUP)
	{
		Logger::Log(DEBUG, "------------->POLLRDHUP");
	}
	if (pfd.revents & POLLWRBAND)
	{
		Logger::Log(DEBUG, "------------->POLLWRBAND");
	}
	// if (pfd.revents & POLLWRNORM)
	// {
	// 	Logger::Log(DEBUG, "------------->POLLWRNORM");
	// }
	if (pfd.revents & POLLPRI)
	{
		Logger::Log(DEBUG, "------------->POLLPRI");
	}
}

void Server::process_message()
{
	// print users_fds_ size
	// Logger::Log(DEBUG, "users_fds_ size: " + to_string_(users_fds_.size()));
	
	int p = poll(users_fds_.data(), users_fds_.size(), timeout_);
	// std::vector<int> to_erase;
	// Logger::Log(DEBUG, "Polling: " + to_string_(p));
	if (p == 0)
	{
		return;
	}
	for (size_t i = 0; i < users_fds_.size(); ++i)
	{
		// Logger::Log(DEBUG, "POLLED " + users_[i]->get_nickname());
		print_poll_revents(users_fds_[i]);
		// users_[i]->send_message("you been POLLED :42\r\n");
		if (users_fds_[i].revents & POLLIN)
		{
			Logger::Log(DEBUG, "User " + users_[i]->get_nickname() + " POLLIN.");
			Response::set_user(users_[i]); // !
			if (users_[i]->read_message() == DISCONNECT)
			{
				users_[i]->set_flag(BREAK);
				Logger::Log(DEBUG, "User " + users_[i]->get_nickname() + " BREAK flag activated (read).");
			}
			else if (handle_message(*(users_[i])) == DISCONNECT)
			{
				users_[i]->set_flag(BREAK);
				Logger::Log(DEBUG, "User " + users_[i]->get_nickname() + " BREAK flag activated.");
			}
			Logger::Log(DEBUG, "User " + users_[i]->get_nickname() + " message processed.");
			users_fds_[i].revents = 0;
		}
		// users_[i]->send_message("POLL flags checked\r\n");
	}
}

int Server::handle_message(User& user)
{
	while (user.get_message().size() > 0 &&
		user.get_message().front()[user.get_message().front().size() - 1] == '\n')
	{
		Message message(user.get_message().front());
		user.pop_message();
		Logger::Log(INFO, message.get_message() + " from " + user.get_nickname());
		if (!(user.get_flags() & REGISTERED) && message.get_command() != "QUIT" && message.get_command() != "PASS" \
			&& message.get_command() != "USER" && message.get_command() != "NICK" \
			&& message.get_command() != "CAP") // shorter way?
		{
			Response::reply(ERR_NOTREGISTERED);
		}
		else
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
			std::cout << "contains_trailing: " << message.get_trailing_flag() << std::endl;
			std::cout << "Trailing: " << message.get_trailing() << std::endl;
			
			Executor executor(this); // efficient to create an instance every time? static-ify? or make it a member?
			int response = executor.execute(message, user); // response confusion with Response !
			// print all users per channel
			std::map<std::string, Channel*>::const_iterator it = channels_.begin();
			for (; it != channels_.end(); ++it)
			{
				std::vector<const User*> users = it->second->get_operators();
				std::cout << ">>>>>>>>>>Channel: " << it->first << std::endl;
				for (size_t i = 0; i < users.size(); ++i)
				{
					std::cout << users[i]->get_nickname() << std::endl;
				}
			}
			std::cout << "response: " << response << std::endl;
			if (response == DISCONNECT)
			{
				return DISCONNECT;
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
				break ;
			}
		}
	}
}

bool Server::contains_nickname(const std::string& nickname) const
{
	for (size_t i = 0; i < users_.size(); ++i)
	{
		Logger::Log(DEBUG, "Checking nickname: " + users_[i]->get_nickname() + " against: " + nickname);
		if (users_[i]->get_nickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

int Server::join_channel(const std::string& name, const std::string& key, User& user) // review with modes
{
	if (channels_.find(name) != channels_.end())
	{
		if ((channels_[name]->get_flags() & USERLIMIT) && channels_[name]->get_users().size() >= channels_[name]->get_user_limit())
		{
			Response::reply(ERR_CHANNELISFULL);
		}
		else if ((channels_[name]->get_flags() & INVITEONLY) && !channels_[name]->is_invited(&user))
		{
			Response::reply(ERR_INVITEONLYCHAN);
		}
		else if ((channels_[name]->get_flags() & CHANNELKEY) && !channels_[name]->is_invited(&user) \
			&& channels_[name]->get_password() != key)
		{
			Response::reply(ERR_BADCHANNELKEY);
		}
		else if (!channels_[name]->contains_nickname(user.get_nickname()))
		{
			Response::set_channel(channels_[name]);
			user.add_channel(*channels_[name]);
			channels_[name]->remove_invite(&user);
			return channels_[name]->add_user(user); // review !
		}
	}
	else
	{
		channels_[name] = new Channel(name, "", user); // already adds user
		user.add_channel(*channels_[name]);
		Response::set_channel(channels_[name]); // or use the one from params_, already set
		// Response::set_user(&user);
		// user.send_message(":" + user.get_prefix() + " JOIN " + name + "\r\n"); //  JOIN message
		
		// channels_[name]->send_message(":" + user.get_prefix() + " JOIN " + name + "\r\n", user, true);
		// channels_[name]->send_message(Response::get_reply(CMD_JOIN), user, true);
		Response::channel_reply(CMD_JOIN, user, *channels_[name]);
		if (channels_[name]->get_topic().size())
		{
			Response::reply(RPL_TOPIC);
			Response::reply(RPL_TOPICWHOTIME);
		}
		Response::reply(RPL_NAMREPLY);
		Response::reply(RPL_ENDOFNAMES);
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
	// print all channels
	// std::map<std::string, Channel*>::const_iterator begin = channels_.begin();
	// std::map<std::string, Channel*>::const_iterator end = channels_.end();
	// for (; begin != end; ++begin)
	// {
	// 	std::cout << begin->first << std::endl;
	// }
	if (channels_.find(channel) != channels_.end())
	{
		// print users
		// std::vector<const User*> users = channels_.at(channel)->get_users();
		// Logger::Log(DEBUG, "Users on channel " + channel);
		// for (size_t i = 0; i < users.size(); ++i)
		// {
		// 	std::cout << users[i]->get_nickname() << std::endl;
		// }
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
}

const bool& Server::is_running() const
{
	return running_;
}