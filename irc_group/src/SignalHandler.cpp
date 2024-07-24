/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 15:52:12 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/24 16:00:31 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SignalHandler.hpp"

SignalHandler::SignalHandler(Server& server) : server_(server)
{
}

SignalHandler::~SignalHandler()
{
}

void SignalHandler::sigint_handler(int signum)
{
    if (signum == SIGINT)
    {
        Logger::Log(INFO, "SIGINT received. Exiting...");
        server_.stop();
    }
}
