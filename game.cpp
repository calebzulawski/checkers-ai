#include "game.h"

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
	while(std::cin >> selection) {
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

			// Move *m, *n;
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
	std::cin.ignore(std::numeric_limits<std::streamsize>::max());
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