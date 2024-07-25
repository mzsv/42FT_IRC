/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ticTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:13:26 by amenses-          #+#    #+#             */
/*   Updated: 2024/07/25 23:18:47 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TicTacToe {
public:
    TicTacToe() : board(3, vector<char>(3, ' ')), currentPlayer('X') {}

    // Display the board
    void displayBoard() const {
        cout << "  1 2 3\n";
        for (size_t i = 0; i < board.size(); ++i) {
            cout << i + 1 << " ";
            for (size_t j = 0; j < board[i].size(); ++j) {
                cout << board[i][j];
                if (j < board[i].size() - 1) cout << "|";
            }
            cout << "\n";
            if (i < board.size() - 1) cout << "  -----\n";
        }
    }

    // Make a move
    bool makeMove(int row, int col) {
        if (row >= 1 && row <= 3 && col >= 1 && col <= 3 && board[row - 1][col - 1] == ' ') {
            board[row - 1][col - 1] = currentPlayer;
            return true;
        }
        return false;
    }

    // Check for a win
    bool checkWin() const {
        // Check rows, columns, and diagonals
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer) return true;
            if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer) return true;
        }
        if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) return true;
        if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer) return true;
        return false;
    }

    // Check for a draw
    bool checkDraw() const {
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == ' ') return false;
            }
        }
        return true;
    }

    // Switch players
    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Get current player
    char getCurrentPlayer() const {
        return currentPlayer;
    }

private:
    vector<vector<char>> board;
    char currentPlayer;
};

int main() {
    TicTacToe game;
    string input;
    int row, col;

    cout << "Welcome to Tic-Tac-Toe! Player X goes first." << endl;
    game.displayBoard();

    while (true) {
        cout << "Player " << game.getCurrentPlayer() << ", enter your move (row and column): ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            row = input[0] - '0';
            col = input[2] - '0';
            if (game.makeMove(row, col)) {
                game.displayBoard();
                if (game.checkWin()) {
                    cout << "Player " << game.getCurrentPlayer() << " wins!" << endl;
                    break;
                }
                if (game.checkDraw()) {
                    cout << "It's a draw!" << endl;
                    break;
                }
                game.switchPlayer();
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        } catch (...) {
            cout << "Invalid input. Please enter row and column in the format 'row col'." << endl;
        }
    }

    cout << "Game over. Thanks for playing!" << endl;
    return 0;
}
