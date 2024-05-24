/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_err_codes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:01:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/05/24 22:14:06 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERR_CODES_HPP
#define IRC_ERR_CODES_HPP

#define ERR_NOSUCHCHANNEL       403
#define ERR_UNKNOWNCOMMAND		421
#define ERR_NOTREGISTERED		451
#define ERR_BADCHANMASK			476
#define ERR_CHANOPRIVSNEEDED	482

#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTERED	462
#define ERR_PASSWDMISMATCH		464
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433

#endif // IRC_ERR_CODES_HPP
