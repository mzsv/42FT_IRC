/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_nickname.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:06:54 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 19:09:13 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

//! Может инкапсулировать в классе...
bool is_valid_nickname(const std::string& nickname)
{
	if (nickname.size() > 9)
	{
		return false;
	}
	return true;
}
