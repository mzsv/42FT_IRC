/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:54:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 15:27:10 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class User;


#include "User.hpp"

class Response
{
  private:

  public:
	static void error(const User& user, int error, const std::string& arg1 = "", const std::string& arg2 = "");
};

#endif // RESPONSE_HPP
