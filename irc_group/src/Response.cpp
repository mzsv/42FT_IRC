/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:55:47 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 16:34:10 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

void Response::error(const User& user, int error, const std::string& arg1, const std::string& arg2)
{
	(void)arg1;
	(void)arg2;
	std::string message = ":" + user.get_server_name() + " ";
	// message += error
	message += " " + user.get_nickname();
	switch (error)
	{
	case ERR_NOTREGISTERED:
		/* code */
		break;

	default:
		break;
	}
}
