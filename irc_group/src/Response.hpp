/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:54:20 by amitcul           #+#    #+#             */
/*   Updated: 2024/06/21 12:55:35 by amenses-         ###   ########.fr       */
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
  static void reply(const User& user, int reply, const std::string& arg1 = "", const std::string& arg2 = "");
};

#endif // RESPONSE_HPP
