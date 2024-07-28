/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2queue.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:00:09 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/18 15:05:44 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <queue>
#include <string>
#include <algorithm>

std::queue<std::string> split2queue(const std::string& str, char sep, bool incl)
{
	std::queue<std::string> res;
	std::string::const_iterator i = str.begin();
	while (i != str.end())
	{
		while (i != str.end() && *i == sep)
		{
			++i;
		}
		std::string::const_iterator j = std::find(i, str.end(), sep);
		if (i != str.end())
		{
			if (incl && j != str.end())
			{
				res.push(std::string(i, j + 1));
			}
			else
			{
				res.push(std::string(i, j));
			}
			i = j;
		}
	}
	return res;
}
