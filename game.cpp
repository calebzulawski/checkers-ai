#include "game.h"

#include <vector>
#include <sstream>
#include <iostream>

#include "constants.h"

void Game::run() {
	Player turn = WHITE;
	size_t turnCount;
	for (turnCount = 1;; turnCount++) {
		std::vector<Move> moves;
		board->possible_moves(turn, moves);

		if (isAI(turn)) {

		} else {
			std::cout << clear_scr;
			board->display();
			std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter move: ";

			std::vector<size_t> input;
			Move *m, *n;
			do {
				get_command(input);
				if (std::cin.eof())
					exit(0);
				m = nullptr;
				for ( size_t i = 0; i < moves.size(); i++ ) {
					if (moves[i].start == input[0] && moves[i].end == input[1])
						m = &moves[i];
				}

				for ( size_t depth = 2; depth < input.size(); depth++ ) {
					n = nullptr;
					if (!m || m->children.size() == 0) {
						m = nullptr;
						break;
					}
					for ( size_t i = 0; i < m->children.size(); i++ ) {
						if (m->children[i].start == input[depth-1] && m->children[i].end == input[depth])
							n = &moves[i];
					}
					m = n;
				}

				if (m) {
					delete board;
					board = new Board(*m->board);
				} else {
					std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << "Enter a valid move: ";
				}
			} while (m == nullptr);
		}
		turnCount++;
		turn = turn == WHITE ? BLACK : WHITE;
	}
}

void Game::get_command(std::vector<size_t> &command) {
	std::string s;
	getline(std::cin,s);
	std::stringstream ss(s);
	int temp;
	while(ss >> temp)
		command.push_back(temp);
}