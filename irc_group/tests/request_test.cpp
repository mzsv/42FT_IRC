/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:55:09 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/04 14:14:43 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "parser.hpp"
#include "server.hpp"
#include "request.hpp"


namespace
{

TEST(KICK_command, parse_kick_command)
{
	std::vector<std::string> input{
		"KICK", "username", "optional_comment"
	};
	Request* request = new Request(input[0]);
	if (!request) return;
	request->set_user(input[1]);
	request->set_comment(input);

	// Parser* parser = Parser::GetInstance();

	// Request* result = parser->parse("KICK username :optional_comment");
	// std::cout << request->get_user() << " " << result->get_user() << std::endl;
	// EXPECT_EQ(request->get_user(), result->get_user());
	// EXPECT_EQ(request->get_comment(), result->get_comment());
}

} // namespace
