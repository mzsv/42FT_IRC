/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:41:52 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/26 17:27:19 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#include <vector>
#include <string>
#include "../includes/to_string_.hpp"
#include "Bot.hpp"

class TicTacToe {
    private:
        TicTacToe();
        TicTacToe(const TicTacToe& obj);
        TicTacToe& operator=(const TicTacToe& obj);

        Bot* bot_;
        std::string target_;
        std::vector<std::string > board_;
        char currentPlayer_;
        
        void display_board() const;
        bool make_move(int row, int col);
        bool check_win() const;
        bool check_draw() const;
        void switch_player();
        char get_current_player() const;
        void output(std::string line) const;

    public:
        TicTacToe(Bot* bot, std::string target);
        ~TicTacToe();

        std::string get_target() const;
        int play_round(std::string arg);
};

#endif // TICTACTOE_HPP
