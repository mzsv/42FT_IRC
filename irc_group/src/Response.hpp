/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:54:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/11 19:31:11 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class User;

#include "User.hpp"
#include "irc_codes.hpp"

class Response
{
  private:
  static std::map<IrcCode, std::string> irc_messages;
  static std::string generate_message(IrcCode code, std::map<std::string, std::string>& params);

  public:
  static std::map<IrcCode, std::string> initialize_irc_messages();
	static void error(const User& user, int error, const std::string& arg1 = "", const std::string& arg2 = "");
  static void reply(const User& user, int reply, const std::string& arg1 = "", const std::string& arg2 = "");
  static void error_reply(const User& user, IrcCode code, std::map<std::string, std::string>& params);

};

#endif // RESPONSE_HPP
