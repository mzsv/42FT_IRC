/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_nickname.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:06:54 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/13 20:28:32 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>

bool is_valid_nickname(const std::string& nick)
{
	if (nick.empty() || nick.length() > 9) {
        return false;
    }
    if (!std::isalpha(nick[0]) && nick[0] != '-' && nick[0] != '[' &&
        nick[0] != ']' && nick[0] != '\\' && nick[0] != '`' && nick[0] != '^' &&
        nick[0] != '{' && nick[0] != '}' && nick[0] != '|') {
        return false;
    }
    for (size_t i = 1; i < nick.length(); ++i) {
        if (!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' &&
            nick[i] != ']' && nick[i] != '\\' && nick[i] != '`' && nick[i] != '^' &&
            nick[i] != '{' && nick[i] != '}' && nick[i] != '|') {
            return false;
        }
    }
    return true;
}
