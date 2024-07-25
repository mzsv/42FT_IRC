/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tolower_str.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:16:48 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 18:45:33 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>

std::string tolower_str(const std::string& str)
{
    std::string lower_str = str;
    for (size_t i = 0; i < lower_str.length(); i++)
    {
        lower_str[i] = std::tolower(lower_str[i]);
    }
    return lower_str;
}
