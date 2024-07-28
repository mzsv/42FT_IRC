/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:53:51 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/29 00:42:14 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

enum LogType
{
	FATAL,
	ERROR,
	WARN,
	INFO,
	DEBUG
};

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
