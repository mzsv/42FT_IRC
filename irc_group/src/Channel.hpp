/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:47:01 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/27 15:20:18 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include "User.hpp"

#define INVITEONLY	0b000001 // 'i' flag in RFC 2811, default: OFF
#define TOPICMODE	0b000010 // 't' flag in RFC 2811, default: ON
#define CHANNELKEY	0b000100 // 'k' flag in RFC 2811, default: off
#define CHANNELOP	0b001000
#define USERLIMIT	0b010000 // 'l' flag in RFC 2811, default: ON
#define NOMSGOUT	0b100000 // 'n' flag in RFC 2811, default: ON

class Channel
{
  private:
	std::string name_;
	std::string password_;
	std::string topic_;
	unsigned short user_limit_;
	unsigned char flags_;
	std::vector<const User*> operators_;
	std::vector<const User*> users_;
	std::set<const User*> invites_;
	time_t topic_time_;
	time_t start_time_;

	/**
     * Forbidden APIs
    */
	Channel();
	Channel(const Channel& obj);
	Channel& operator=(const Channel& obj);

  public:
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
	int add_user(const User& user);
	void add_operator(std::string nickname);
	void remove_operator(std::string nickname);
	void reset_flag(unsigned char flag);
	bool is_invited(const User* user) const;
	void add_invite(const User* user);
	void remove_invite(const User* user);
	void clear_invites();
};

#endif // CHANNEL_HPP
