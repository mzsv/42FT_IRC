/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_argv_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:20:21 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/04 13:51:45 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <cstring>

bool is_valid_data(char **argv);

namespace
{
TEST(InputDataValidation, is_valid_data)
{
	char **argv = new char*[3];

	argv[0] = new char[strlen("binName") + 1];
	argv[1] = new char[strlen("4242") + 1];
	argv[2] = new char[strlen("password") + 1];
	std::strcpy(argv[0], "binName");
    std::strcpy(argv[1], "4242");
    std::strcpy(argv[2], "password");

	EXPECT_EQ(true, is_valid_data(argv));

	std::strcpy(argv[1], "-100");
	EXPECT_EQ(false, is_valid_data(argv));

	std::strcpy(argv[1], "1000");
	EXPECT_EQ(false, is_valid_data(argv));

	std::strcpy(argv[1], "1bc2");
	EXPECT_EQ(false, is_valid_data(argv));

	argv[1] = new char[strlen("12345678")];
	std::strcpy(argv[1], "12345678");
	EXPECT_EQ(false, is_valid_data(argv));
}

} // namespace
