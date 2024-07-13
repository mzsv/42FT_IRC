/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:54:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/13 16:58:47 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class User;

#include "User.hpp"
#include "irc_codes.hpp"

class Response // should it really be static?
{
  private:
  static std::map<IrcCode, std::string> irc_messages_;
  static std::map<std::string, std::string> params_;
  static std::string generate_message(IrcCode code);

  public:
  static std::map<IrcCode, std::string> initialize_irc_messages();
  static void add_param(const std::string& key, const std::string& value);
  static std::map<std::string, std::string>& get_params(); // is it used ?
  static void params_clear();
	static void error(const User& user, int error, const std::string& arg1 = "", const std::string& arg2 = "");
  static void reply(const User& user, int reply, const std::string& arg1 = "", const std::string& arg2 = "");
  static void error_reply(const User& user, IrcCode code);

};

#endif // RESPONSE_HPP
