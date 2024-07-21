/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stoi_.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:31:03 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/21 16:44:45 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STON_HPP
#define STON_HPP

#include <string>
#include <sstream>

template <typename T>
T ston(const std::string str)
{
    std::istringstream ss(str);
    T n;
    ss >> n;
    if (ss.fail())
    {
        n = 0;
        // throw std::invalid_argument("stoi_: invalid argument");
    }
    return n;
}

#endif // STON_HPP
