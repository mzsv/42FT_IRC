/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_codes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:58:54 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/13 20:08:13 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CODES_HPP

# include <map>
# include <string>

enum IrcCode
{
    RPL_CHANNELMODEIS,
    RPL_CREATIONTIME,
    RPL_NOTOPIC,
    RPL_TOPIC,
    RPL_TOPICWHOTIME,
    RPL_INVITING,
    RPL_NAMREPLY,
    RPL_ENDOFNAMES,
    ERR_NOSUCHNICK,
    ERR_NOSUCHCHANNEL,
    ERR_CANNOTSENDTOCHAN,
    ERR_NOORIGIN,
    ERR_NORECIPIENT,
    ERR_NOTEXTTOSEND,
    ERR_UNKNOWNCOMMAND,
    ERR_USERNOTINCHANNEL,
    ERR_NOTONCHANNEL,
    ERR_USERONCHANNEL,
    ERR_NOTREGISTERED,
    ERR_CHANNELISFULL,
    ERR_INVITEONLYCHAN,
    ERR_BADCHANNELKEY,
    ERR_BADCHANMASK,
    ERR_CHANOPRIVSNEEDED,
    ERR_NEEDMOREPARAMS,
    ERR_ALREADYREGISTERED,
    ERR_PASSWDMISMATCH,
    ERR_ERRONEUSNICKNAME,
    ERR_NICKNAMEINUSE,
    ERR_KEYSET,
    ERR_UNKNOWNMODE,
    ERR_UMODEUNKNOWNFLAG,
    ERR_INVALIDMODEPARAM,
};

#endif // IRC_CODES_HPP
