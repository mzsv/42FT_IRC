/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 15:45:16 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/24 17:48:54 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

#include <signal.h>
#include "Server.hpp"

class SignalHandler
{
    private:
        Server& server_;
        static SignalHandler* instance_;

        SignalHandler();
        SignalHandler(const SignalHandler& obj);
        SignalHandler& operator=(const SignalHandler& obj);
        
    public:
        SignalHandler(Server& server_);
        ~SignalHandler();
        
        static void sigint_handler(int signum);
        void config();
};

#endif // SIGNALHANDLER_HPP
