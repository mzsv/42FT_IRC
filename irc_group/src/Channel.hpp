/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:47:01 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/18 22:57:07 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"

// not dealing with private, secret or moderated channels in this project !
// #define PRIVATE		0b000001 // requires key?? assuming that
// #define SECRET		0b000010
// #define MODERATED	0b000100
// #define INVITEONLY	0b001000
// #define TOPICSET	0b010000 // topic set by operator only mode? assuming that. maybe change names?
#define NOMSGOUT	0b100000 // ?

#define INVITEONLY	0b000001 // use enum instead? cleaner right?
#define TOPICMODE	0b000010
#define CHANNELKEY	0b000100
#define CHANNELOP	0b001000 //  is this needed ? is it a flag for channel modes?
#define USERLIMIT	0b010000

class Channel
{
  private:
	std::string name_;
	std::string password_; // key !
	std::string topic_;
	unsigned short user_limit_; // safer with int ?! also protocol mentions int
	unsigned char flags_;

	std::vector<const User*> operators_;
	std::vector<const User*> users_; // not efficient for insertion/deletion of elements; set instead?
	std::vector<const User*> speakers_;

	time_t topic_time_; // needed for TOPIC
	time_t start_time_;

	Channel();
	Channel(const Channel& channel);
	Channel& operator=(const Channel& channel);

  public:
  // password not required at creation, only set after key mode is set. right?
	Channel(const std::string& name, const std::string& pass, const User& creator);
	~Channel();

	/**
	 * Getters
	*/
	const std::string& get_name() const;
	const std::string& get_password() const;
	const std::string& get_topic() const;
	unsigned char get_flags() const;
	const unsigned short& get_user_limit() const;

	const time_t& get_topic_time() const;
	const std::vector<const User*>& get_users() const;
	const std::vector<const User*>& get_operators() const;
	const User* get_user(const std::string& nickname) const;
	const time_t& get_start_time() const;

	/**
	 * Setters
	*/
	void set_topic(const User& user, std::string topic);
	void set_password(const User& user, std::string password);

	void set_topic_time();
	void set_flag(unsigned char flag);
	void set_user_limit(unsigned short limit);
	
	/**
	 * Funcs
	*/
	bool contains_nickname(const std::string& nickname) const;
	bool is_empty();
	void disconnect(const User& user);
	bool is_operator(const User& user) const;
	bool is_operator(const std::string& nickname) const;
	void send_message(const std::string& message, const User& user, bool include_user) const;

	int add_user(const User& user); // !
	void add_operator(std::string nickname);
	void remove_operator(std::string nickname);
	void reset_flag(unsigned char flag);
};

#endif // CHANNEL_HPP
