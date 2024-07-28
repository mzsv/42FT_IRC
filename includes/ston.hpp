/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ston.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:31:03 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/26 19:45:44 by amenses-         ###   ########.fr       */
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
    }
    return n;
}

#endif // STON_HPP
