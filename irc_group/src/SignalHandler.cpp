/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 15:52:12 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/24 18:00:37 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SignalHandler.hpp"
#include "Logger.hpp"
#include "Server.hpp"

SignalHandler* SignalHandler::instance_ = NULL;

SignalHandler::SignalHandler(Server& server) : server_(server)
{
    instance_ = this;
    config();
}

SignalHandler::~SignalHandler()
{
}

void SignalHandler::sigint_handler(int signum)
{
    if (signum == SIGINT)
    {
        Logger::Log(INFO, "SIGINT received. Exiting...");
        SignalHandler::instance_->server_.stop();
    }
}

void SignalHandler::config()
{
    struct sigaction sa_int, sa_quit;

    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        Logger::Log(FATAL, "Sigaction() failed");
        server_.stop();
    }
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
    {
        Logger::Log(FATAL, "Sigaction() failed");
        server_.stop();
    }
}
