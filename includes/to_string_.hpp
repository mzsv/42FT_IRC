/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_string_.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:51:53 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/15 17:00:13 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TO_STRING__HPP
#define TO_STRING__HPP

#include <string>
#include <sstream>

template <typename T>
std::string to_string_(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

#endif
