/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:47:01 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 17:49:27 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"


#define PRIVATE		0b000001
#define SECRET		0b000010
#define MODERATED	0b000100
#define INVITEONLY	0b001000
#define TOPICSET	0b010000
#define NOMSGOUT	0b100000


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
	std::vector<const User*> speakers_;

	Channel();
	Channel(const Channel& channel);
	Channel& operator=(const Channel& channel);

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

	/**
	 * Setters
	*/
	void set_topic(const User& user, std::string topic);

	/**
	 * Funcs
	*/
	bool contains_nickname(const std::string& nickname) const;
	bool is_empty();
	void disconnect(User& user);
	bool is_operator(const User& user) const;
	void send_message(const std::string& message, const User& user, bool include_user) const;
};

#endif // CHANNEL_HPP
