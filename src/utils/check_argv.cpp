/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_argv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:44:06 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/23 10:12:55 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>

bool is_valid_data(char **argv)
{
	long int port_tmp = atol(argv[1]);
	if (port_tmp <= 1023 || port_tmp >= std::numeric_limits<short>::max())
        return false;
    return true;
}
