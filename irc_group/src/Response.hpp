/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:54:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/19 23:19:46 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class User;

#include <iomanip>
#include <algorithm>
#include <fstream>
#include "User.hpp"
#include "irc_codes.hpp"

typedef std::string (*RplFunctionPointer)(IrcCode); // no this for static functions

class Response // should it really be static?
{
  private:
  static const User* user_;
  static const User* target_user_;
  static const Channel* channel_;
  static std::string command_;
  static std::map<IrcCode, std::string> irc_messages_;
  static std::map<std::string, std::string> params_;
  static std::map<IrcCode, RplFunctionPointer> rpl_functions_;
  static std::string generate_message(IrcCode code);
  
  
  static std::string rpl_welcome(IrcCode code);
  static std::string rpl_yourhost(IrcCode code);
  static std::string rpl_created(IrcCode code);
  static std::string rpl_myinfo(IrcCode code);
  static std::string rpl_isupport(IrcCode code);
  static std::string rpl_luserclient(IrcCode code);
  static std::string rpl_luserop(IrcCode code);
  static std::string rpl_luserunknown(IrcCode code);
  static std::string rpl_luserchannels(IrcCode code);
  static std::string rpl_luserme(IrcCode code);
  static std::string rpl_localusers(IrcCode code);
  static std::string rpl_globalusers(IrcCode code);
  static std::string rpl_whoisuser(IrcCode code);
  static std::string rpl_whoisserver(IrcCode code);
  static std::string rpl_endofwho(IrcCode code);
  static std::string rpl_whoisidle(IrcCode code);
  static std::string rpl_endofwhois(IrcCode code);
  static std::string rpl_channelmodeis(IrcCode code);
  static std::string rpl_creationtime(IrcCode code);
  static std::string rpl_notopic(IrcCode code);
  static std::string rpl_topic(IrcCode code);
  static std::string rpl_topicwhotime(IrcCode code);
  static std::string rpl_inviting(IrcCode code);
  static std::string rpl_whoreply(IrcCode code);
  static std::string rpl_namreply(IrcCode code);
  static std::string rpl_endofnames(IrcCode code);
  static std::string rpl_motd(IrcCode code);
  static std::string rpl_motdstart(IrcCode code);
  static std::string rpl_endofmotd(IrcCode code);
  static std::string rpl_whoishost(IrcCode code);
  static std::string rpl_whoismodes(IrcCode code);


  public:
  static void set_targets(const User* user, const Channel* channel); //  still needed ?
  static void set_user(const User* user);
  static void set_target_user(const User* user);
  static void set_channel(const Channel* channel);
  static void set_command(const std::string& command);
  static std::map<IrcCode, std::string> initialize_irc_messages();
  static std::map<IrcCode, RplFunctionPointer> initialize_rpl_functions();
  static void add_param(const std::string& key, const std::string& value);
  static std::map<std::string, std::string>& get_params(); // is it used ?
  static void params_clear();
	// static void error(const User& user, int error, const std::string& arg1 = "", const std::string& arg2 = "");
  // static void reply(const User& user, int reply, const std::string& arg1 = "", const std::string& arg2 = "");
  static void error_reply(IrcCode code);
  static void reply(IrcCode code);
  static void reset();
};

#endif // RESPONSE_HPP
