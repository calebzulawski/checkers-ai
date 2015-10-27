#include "game.h"

#include <vector>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>

#include "constants.h"

void Game::prompt() {
	std::cout << "Choose an option: " << std::endl;
	std::cout << "\t(1)\tAI vs player, player plays white" << std::endl;
	std::cout << "\t(2)\tAI vs Player, player plays black" << std::endl;
	std::cout << "\t(3)\tAI vs AI" << std::endl;
	int selection;
	std::string s;
	getline(std::cin, s);
	std::stringstream ss(s);
	while(ss >> selection) {
		std::cout << "\t>" << std::endl;
		if (selection == 1) {
			whiteIsAi = false;
			blackIsAi = true;
			break;
		} else if (selection == 2) {
			whiteIsAi = true;
			blackIsAi = false;
			break;
		} else if (selection == 3) {
			whiteIsAi = true;
			blackIsAi = true;
			break;
		}
	}
}

void Game::get_command(std::vector<size_t> &command) {
	std::string s;
	getline(std::cin, s);
	std::stringstream ss(s);
	int temp;
	while(ss >> temp)
		command.push_back(temp);
}

bool Game::find_move(std::vector<size_t> &input, std::vector<Move> &moves, size_t depth, Move &foundMove) {
	if ( ((int)input.size() - 1) < (int)(depth + 1))
		return false;
	for (auto move : moves) {
		if (move.start == input[depth] && move.end == input[depth+1]) {
			if (move.children.size() == 0) {
				if (input.size()-1 == depth+1) {
					foundMove = move;
					return true;
				} else {
					return false;
				}
			} else {
				return find_move(input, move.children, depth+1, foundMove);
			}
		}
	}
	return false;
}

void Game::load(char *filename) {
	board->clear();
	std::ifstream ifs(filename);
	size_t val;
	for (size_t i = 0; i < 32; i++) {
		ifs >> val;
		if (ifs.eof())
			exit(-1);
		if (val == 1) {
			board->white_pieces->set(i);
		}
		else if (val == 2) {
			board->black_pieces->set(i);
		}
		else if (val == 3) {
			board->white_pieces->set(i);
			board->white_kings->set(i);
		}
		else if (val == 4) {
			board->black_pieces->set(i);
			board->black_kings->set(i);
		}
	}
	ifs >> val;
	if (ifs.eof())
		exit(-1);

	if (val == 1)
		turn = WHITE;
	else
		turn = BLACK;

	ifs >> val;
	//eventually set this to search time
}

void Game::run() {
	size_t turnCount;
	for (turnCount = 1;; turnCount++) {
		std::cout << clear_scr;
		board->display();

		std::vector<Move> moves;
		board->possible_moves(turn, moves);
		
		if (moves.size() == 0)
			break;

		if (isAI(turn)) {
			split_moves(moves);
			Move bestMove;
			if (moves.size() == 1)
				bestMove = moves.front();
			else
				board->alpha_beta_start(10, turn, moves, bestMove);
			*board = *bestMove.board;
			printf("Press enter to continue...");
			std::cin.get();
		} else {
			std::vector<size_t> path;
			split_moves(moves, &path);
			std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter move: ";

			while(true) {
				std::vector<size_t> input;
				get_command(input);
				if (std::cin.eof())
					exit(0);

				Move m;
				if (find_move(input, moves, 0, m)) {
					*board = *m.board;
					break;
				} else {
					std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter a valid move: ";
					continue;
				}
			}
		}
		turnCount++;
		turn = turn == WHITE ? BLACK : WHITE;
	}

	std::cout << (turn == WHITE ? "BLACK" : "WHITE") << " wins!" << std::endl << "Game completed in " << turnCount << " turns." << std::endl;
}
