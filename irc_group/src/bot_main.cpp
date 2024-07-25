/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:19:38 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 19:43:18 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        Logger::Log(ERROR, "Usage: ./bot <port> <password>");
        return 1;
    }
    Bot bot(ston<int>(argv[1]), argv[2]);
    bot.run();
    return 0;
}
