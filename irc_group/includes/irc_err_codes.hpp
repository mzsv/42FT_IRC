/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_err_codes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:01:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/06/14 19:55:50 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERR_CODES_HPP
#define IRC_ERR_CODES_HPP

#define ERR_NOSUCHNICK			401
#define ERR_NOSUCHCHANNEL       403
#define ERR_CANNOTSENDTOCHAN	404
#define ERR_NORECIPIENT			411
#define ERR_NOTEXTTOSEND        412
#define ERR_UNKNOWNCOMMAND		421
#define ERR_USERNOTINCHANNEL    441
#define ERR_NOTONCHANNEL        442
#define ERR_USERONCHANNEL       443
#define ERR_NOTREGISTERED		451
#define ERR_CHANNELISFULL		471
#define ERR_INVITEONLYCHAN		473
#define ERR_BADCHANNELKEY		475
#define ERR_BADCHANMASK			476
#define ERR_CHANOPRIVSNEEDED	482

#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTERED	462
#define ERR_PASSWDMISMATCH		464
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433
#define ERR_KEYSET              467
#define ERR_UNKNOWNMODE         472
#define ERR_UMODEUNKNOWNFLAG    501

#endif // IRC_ERR_CODES_HPP
