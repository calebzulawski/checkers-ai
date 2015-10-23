#include "game.h"

#include <vector>
#include <sstream>
#include <iostream>

#include "constants.h"

void Game::run() {
	Player turn = WHITE;
	size_t turnCount;
	for (turnCount = 1;; turnCount++) {
		std::cout << clear_scr;
		board->display();

		std::vector<Move> moves;
		board->possible_moves(turn, moves);
		if (moves.size() == 0)
			break;

		if (isAI(turn)) {
			Move bestMove;
			if (moves.size() == 1)
				bestMove = moves.front();
			else
				board->alpha_beta_start(10, turn, moves, bestMove);
			*board = *bestMove.board;
			//printf("Enter to continue...");
			//std::cin.get();
		} else {

			std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter move: ";

			std::vector<size_t> input;
			// Move *m, *n;
			while(true) {
				get_command(input);
				if (std::cin.eof())
					exit(0);

				Move m;
				if (find_move(input, moves, 0, m)) {
					*board = *m.board;
				} else {
					std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter a valid move: ";
					continue;
				}
				// m = nullptr;
				// for ( size_t i = 0; i < moves.size(); i++ ) {
				// 	if (moves[i].start == input[0] && moves[i].end == input[1])
				// 		m = &moves[i];
				// }

				// for ( size_t depth = 2; depth < input.size(); depth++ ) {
				// 	n = nullptr;
				// 	if (!m || m->children.size() == 0) {
				// 		m = nullptr;
				// 		break;
				// 	}
				// 	for ( size_t i = 0; i < m->children.size(); i++ ) {
				// 		if (m->children[i].start == input[depth-1] && m->children[i].end == input[depth])
				// 			n = &moves[i];
				// 	}
				// 	m = n;
				// }

				// // find incomplete jump
				// if (!m || m->children.size() != 0) {
				// 	std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter a valid move: ";
				// 	continue;
				// }
				// *board = *m->board;
			}
		}
		turnCount++;
		turn = turn == WHITE ? BLACK : WHITE;
	}

	std::cout << (turn == WHITE ? "BLACK" : "WHITE") << " wins!" << std::endl << "Game completed in " << turnCount << " turns." << std::endl;
}

void Game::get_command(std::vector<size_t> &command) {
	std::string s;
	getline(std::cin,s);
	std::stringstream ss(s);
	int temp;
	while(ss >> temp)
		command.push_back(temp);
}

bool Game::find_move(std::vector<size_t> &input, std::vector<Move> &moves, size_t depth, Move &foundMove) {
	if ( (input.size() - 1) < (depth + 1))
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