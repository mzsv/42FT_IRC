/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_codes_test.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:01:22 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/11 18:20:23 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "irc_codes.hpp"

std::string generateReply(IrcCode code, std::map<std::string, std::string>& params)
{
    std::string message = IRC_MESSAGES[code];
    std::map<std::string, std::string>::iterator it;

    for (it = params.begin(); it != params.end(); ++it)
    {
        std::string placeholder = "{" + it->first + "}";
        size_t pos = message.find(placeholder);
        while (pos != std::string::npos)
        {
            message.replace(pos, placeholder.size(), it->second);
            pos = message.find(placeholder);
        }
    }
    return message;
}

int main()
{
    std::map<std::string, std::string> params;
    params["nickname"] = "amenses-";
    params["channel"] = "#42";
    params["command"] = "PRIVMSG";
    
    std::cout << generateReply(ERR_NOSUCHNICK, params) << std::endl;
    std::cout << generateReply(ERR_NOSUCHCHANNEL, params) << std::endl;
    std::cout << generateReply(ERR_CANNOTSENDTOCHAN, params) << std::endl;
    return 0;
}
