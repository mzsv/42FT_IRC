/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_nickname.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:06:54 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/21 17:53:14 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>

static bool is_special(char c)
{
    return (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D); // "[", "]", "\", "`", "_", "^", "{", "|", "}"
}

bool is_valid_nickname(const std::string &nickname)
{
    // Check the length constraints
    if (nickname.empty() || nickname.length() > 9)
    {
        return false;
    }
    // Check the first character
    if (!isalpha(nickname[0]) && !is_special(nickname[0]))
    {
        return false;
    }
    // Check the remaining characters
    for (size_t i = 1; i < nickname.length(); ++i)
    {
        char c = nickname[i];
        if (!isalpha(c) && !isdigit(c) && !is_special(c) && c != '-')
        {
            return false;
        }
    }
    return true;
}

static bool is_valid_chanstringchar(char c)
{
    return (c >= 0x01 && c <= 0x09) || \
           (c >= 0x0B && c <= 0x0C) || \
           (c >= 0x0E && c <= 0x1F) || \
           (c >= 0x21 && c <= 0x2B) || \
           (c >= 0x2D && c <= 0x39) || \
           (c >= 0x3B);
}

bool is_valid_channel(const std::string& channel)
{
    if (channel.empty() || channel[0] != '#')
    {
        return false;
    }
    bool colonFound = false;
    for (size_t i = 1; i < channel.length(); ++i)
    {
        char c = channel[i];
        if (c == ':')
        {
            if (colonFound)
            {
                return false; // Only one colon is allowed
            }
            colonFound = true;
        } else if (!is_valid_chanstringchar(c))
        {
            return false;
        }
    }
    return true;
}

static bool is_valid_key_char(char c)
{
    return (c >= 0x01 && c <= 0x05) || \
           (c >= 0x07 && c <= 0x08) || \
           (c == 0x0C) || \
           (c >= 0x0E && c <= 0x1F) || \
           (c >= 0x21 && c <= 0x7F);
}

bool is_valid_key(const std::string& key) // add exceptions explaining reason !
{
    if (key.empty() || key.length() > 23)
    {
        return false;
    }
    for (size_t i = 0; i < key.size(); ++i)
    {
        char c = key[i];
        if (!is_valid_key_char(c))
        {
            return false;
        }
    }
    return true;
}

bool is_valid_username(const std::string &username)
{
    char c;

    for (size_t i = 0; i < username.size(); ++i)
    {
        c = username[i];
        if (!((c >= 0x01 && c <= 0x09) || 
              (c >= 0x0B && c <= 0x0C) || 
              (c >= 0x0E && c <= 0x1F) || 
              (c >= 0x21 && c <= 0x3F) || 
              (c >= 0x41)))
        {
            return false;
        }
    }
    return true;
}
