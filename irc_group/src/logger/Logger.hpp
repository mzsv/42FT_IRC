/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:53:51 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/08 18:09:01 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#include "log_types.hpp"

/// @brief Logger class
class Logger
{
private:
	Logger();
	Logger(const Logger& logger);
	Logger& operator=(const Logger& logger);

public:
	static void Log(const LogType type, const std::string& msg);
};

#endif // LOGGER_HPP
