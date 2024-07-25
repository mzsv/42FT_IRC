/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:41:31 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/26 00:49:31 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TicTacToe.hpp"

TicTacToe::TicTacToe(Bot* bot, std::string target) :
    bot_(bot), target_(target),
    board_(3, std::string(3, ' ')), currentPlayer_('X')
{
    output("Game On! Player X starts. Make a move! Type 'play <row> <col>'\n");
    display_board();
}

TicTacToe::~TicTacToe()
{
}

void TicTacToe::display_board() const
{
    output("  1 2 3\n");
    for (size_t i = 0; i < board_.size(); ++i)
    {
        output(to_string_(i + 1) + " ");
        for (size_t j = 0; j < board_[i].size(); ++j)
        {
            output(std::string(1, board_[i][j]));
            if (j < board_[i].size() - 1)
            {
                output("|");
            }
        }
        output("\n");
        if (i < board_.size() - 1)
        {
            output("  -----\n");
        }
    }
    // output("what is the next move? (play <row> <col>)\n"); // !
}

bool TicTacToe::make_move(int row, int col)
{
    if (row >= 1 && row <= 3 && col >= 1 && col <= 3 && board_[row - 1][col - 1] == ' ')
    {
        board_[row - 1][col - 1] = currentPlayer_;
        return true;
    }
    return false;
}

bool TicTacToe::check_win() const
{
    for (size_t i = 0; i < 3; ++i)
    {
        if (board_[i][0] == currentPlayer_ && board_[i][1] == currentPlayer_ && board_[i][2] == currentPlayer_)
        {
            return true;
        }
        if (board_[0][i] == currentPlayer_ && board_[1][i] == currentPlayer_ && board_[2][i] == currentPlayer_)
        {
            return true;
        }
    }
    if (board_[0][0] == currentPlayer_ && board_[1][1] == currentPlayer_ && board_[2][2] == currentPlayer_)
    {
        return true;
    }
    if (board_[0][2] == currentPlayer_ && board_[1][1] == currentPlayer_ && board_[2][0] == currentPlayer_)
    {
        return true;
    }
    return false;
}

bool TicTacToe::check_draw() const
{
    for (size_t i = 0; i < board_.size(); ++i)
    {
        for (size_t j = 0; j < board_[i].size(); ++j)
        {
            if (board_[i][j] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}

void TicTacToe::switch_player()
{
    currentPlayer_ = (currentPlayer_ == 'X') ? 'O' : 'X';
}

char TicTacToe::get_current_player() const
{
    return currentPlayer_;
}

void TicTacToe::output(std::string line) const
{
    bot_->send_to(target_, line);
}

int TicTacToe::play_round(std::string arg)
{
    int row;
    int col;

    // while (true)
    // {
        // bot_->receive_message();
        // if (messages_.size() > 0)
        // {
        //     Message message = messages_.front();
        //     messages_.pop();
        //     if (message.get_command() == "PRIVMSG")
        //     {
        //         std::string msg = message.get_trailing();
        //         if (msg.find("play ") == 0)
        //         {
    std::string play = arg.substr(6);
    std::istringstream iss(play);
    iss >> row >> col;
    if (make_move(row, col))
    {
        display_board();
        if (check_win())
        {
            output("Player " + std::string(1, currentPlayer_) + " wins!\n");
            return 1;
            // break ;
        }
        if (check_draw())
        {
            output("It's a draw!\n");
            return 1;
            // break ;
        }
        switch_player();
    }
    else
    {
        output("Invalid move!\n");
    }
    output("Player " + std::string(1, currentPlayer_) + "'s turn. Make a move! (play <row> <col>)\n");
    //             }
    //         }
    //     }
    // }
    return 0;
}
