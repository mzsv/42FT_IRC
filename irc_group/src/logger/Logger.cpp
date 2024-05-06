/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amitcul <amitcul@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:56:54 by amitcul           #+#    #+#             */
/*   Updated: 2024/01/22 20:31:50 by amitcul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "colors.hpp"


#define FATAL(msg)  std::cout << ANSI_COLOR_DARK_RED << "FATAL: " << msg << ANSI_COLOR_RESET << std::endl
#define ERROR(msg)  std::cout << ANSI_COLOR_RED << "ERROR: " << msg << ANSI_COLOR_RESET << std::endl
#define WARN(msg)   std::cout << ANSI_COLOR_YELLOW << "WARN: " << msg << ANSI_COLOR_RESET << std::endl
#define INFO(msg)   std::cout << ANSI_COLOR_GREEN << "INFO: " << msg << ANSI_COLOR_RESET << std::endl
#define DEBUG__(msg)  std::cout << ANSI_COLOR_BLUE << "DEBUG: " << msg << ANSI_COLOR_RESET << std::endl


void Logger::Log(const LogType type, const std::string& msg)
{
	switch (type)
	{
	case FATAL:
		FATAL(msg);
		break;
	case ERROR:
		ERROR(msg);
		break;
	case WARN:
		WARN(msg);
		break;
	case INFO:
		INFO(msg);
		break;
	case DEBUG:
		#ifdef DEBUG
		DEBUG__(msg);
		#endif
		break;
	}
}
