/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:47:27 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/31 16:39:15 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <queue>

std::queue<std::string> split2queue(const std::string& str, char sep, bool incl);

class Message
{
  private:
	std::string message_;
	std::string command_;
	std::string prefix_;
	std::vector<std::string> arguments_;
	bool trailing_flag_;
	// std::string trailing_;

  public:
	Message(const std::string& message);

	const std::string& get_message() const;
	const std::string& get_command() const;
	const std::string& get_prefix() const;
	const std::vector<std::string>& get_arguments() const;
	bool get_trailing_flag() const;
	const std::string get_trailing() const;

	// bool contains_trailing() const;
};

#endif // MESSAGE_HPP
