/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:47:27 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/27 13:54:53 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <queue>
#include "Logger.hpp"

std::queue<std::string> split2queue(const std::string& str, char sep, bool incl);

class Message
{
  private:
	std::string message_;
	std::string command_;
	std::string prefix_;
	std::vector<std::string> arguments_;
	bool trailing_flag_;

	/**
     * Forbidden APIs
    */
   	Message();
	Message(const Message& obj);
	Message& operator=(const Message& obj);

  public:
	Message(const std::string& message);
	~Message();

	/**
	 * Getters
	*/
	const std::string& get_message() const;
	const std::string& get_command() const;
	const std::string& get_prefix() const;
	const std::vector<std::string>& get_arguments() const;
	bool get_trailing_flag() const;
	const std::string get_trailing() const;
};

#endif // MESSAGE_HPP
