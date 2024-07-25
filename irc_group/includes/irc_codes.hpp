/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_codes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:58:54 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 18:39:46 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CODES_HPP

# include <map>
# include <string>

enum IrcCode
{
    RPL_WELCOME = 1,
    RPL_YOURHOST = 2,
    RPL_CREATED = 3,
    RPL_MYINFO = 4,
    RPL_ISUPPORT = 5,
    RPL_LUSERCLIENT = 251,
    RPL_LUSEROP = 252,
    RPL_LUSERUNKNOWN = 253,
    RPL_LUSERCHANNELS = 254,
    RPL_LUSERME = 255,
    RPL_LOCALUSERS = 265,
    RPL_GLOBALUSERS = 266, // redundant (same as LOCALUSERS) because it's only one server
    RPL_WHOISUSER = 311,
    RPL_WHOISSERVER = 312,
    RPL_ENDOFWHO = 315,
    RPL_WHOISIDLE = 317,
    RPL_ENDOFWHOIS = 318,
    RPL_CHANNELMODEIS = 324,
    RPL_CREATIONTIME = 329,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_TOPICWHOTIME = 333,
    RPL_INVITING = 341,
    RPL_WHOREPLY = 352,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    RPL_MOTD = 372,
    RPL_MOTDSTART = 375,
    RPL_ENDOFMOTD = 376,
    RPL_WHOISHOST = 378,
    RPL_WHOISMODES = 379,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_NOORIGIN = 409,
    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NOMOTD = 422,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443,
    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTERED = 462,
    ERR_PASSWDMISMATCH = 464,
    ERR_KEYSET = 467,
    ERR_CHANNELISFULL = 471,
    ERR_UNKNOWNMODE = 472,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_BADCHANMASK = 476,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_UMODEUNKNOWNFLAG = 501,
    ERR_INVALIDKEY = 525,
    ERR_INVALIDMODEPARAM = 696,
    CMD_ERROR = 4242,
    CMD_JOIN = 4243,
    CMD_INVITE = 4244,
    CMD_QUIT = 4245,
    CMD_PART = 4246,
    CMD_KICK = 4247,
    CMD_MODE = 4248,
    CMD_PRIVMSG = 4249,
    CMD_NOTICE = 4250,
    CMD_PONG = 4251,
};

#endif // IRC_CODES_HPP
