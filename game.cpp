#include "game.h"

#include <cstdlib>
#include <vector>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include <thread>

#include "constants.h"

void Game::prompt() {
	int selection;
	std::string s;
	std::cout << "Choose an option:" << std::endl;
	std::cout << "\t(1)\tAI vs player, player plays white" << std::endl;
	std::cout << "\t(2)\tAI vs Player, player plays black" << std::endl;
	std::cout << "\t(3)\tAI vs AI" << std::endl;
	std::cout << "> ";
	getline(std::cin, s);
	std::stringstream ss(s);
	while(ss >> selection) {
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
		else {
			std::cout << "> " << std::endl;
		}
	}

	while (searchTime == -1) {
		std::cout << "Enter an AI search time limit:" << std::endl;
		std::cout << "> ";
		getline(std::cin, s);
		ss.str(s);
		ss.clear();
		std::cout << ss.str() << std::endl;
		ss >> searchTime;
	}
}

void Game::list_moves(std::vector<Move> &v) {
	for (size_t i = 0; i < v.size(); i++) {
		std::cout << i << ")\t";
		v[i].display();
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

	ifs >> searchTime;
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

		if (isAI(turn))
			takeTurn_AI(moves);
		else
			takeTurn_player(moves);

		turnCount++;
		turn = (turn == WHITE) ? BLACK : WHITE;
	}

	std::cout << (turn == WHITE ? "BLACK" : "WHITE") << " wins!" << std::endl << "Game completed in " << turnCount << " turns." << std::endl;
}

void Game::takeTurn_AI(std::vector<Move> &moves) {
	split_moves(moves, false);
	Move bestMove;
	if (moves.size() == 1) {
		std::cout << "Selecting only posible move." << std::endl;
		bestMove = moves.front();
	} else {
		bool trigger = false;
		std::thread timerThread(timer, &trigger, searchTime);
		auto start_time = std::chrono::high_resolution_clock::now();
		size_t depth = board->iterative_deepening(turn, moves, bestMove, trigger);
		auto end_time = std::chrono::high_resolution_clock::now();
		auto diff_time = 1e-3 * std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		timerThread.join();
		if (depth == 0) {
			std::cout << "Was not able to search the root depth in this time!" << std::endl;
			bestMove = moves[rand() % moves.size()];
		} else {
			std::cout << "Searched to depth " << depth << " in " << diff_time << " seconds." << std::endl;
		}
	}
	*board = *bestMove.board;
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
}

void Game::takeTurn_player(std::vector<Move> &moves) {
	std::vector<size_t> path;
	split_moves(moves, true);
	list_moves(moves);
	std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << " Enter move: ";

	while(true) {
		std::string s;
		getline(std::cin, s);
		std::stringstream ss(s);
		size_t index = 1000;

		if (std::cin.eof())
			exit(0);

		if((ss >> index) && !ss.rdbuf()->in_avail() && index < moves.size()) {
			*board = *moves[index].board;
			break;
		} else {
			std::cout << (turn == WHITE ? "(WHITE)" : "(BLACK)") << " Enter a valid move: ";
			continue;
		}
	}
}

void timer(bool *trigger, float searchTime) {
	const float timeFraction = 0.95;
	auto start_time = std::chrono::high_resolution_clock::now();
	while(!*trigger) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		auto end_time = std::chrono::high_resolution_clock::now();
		auto diff_time = 1e-3 * std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		if ( diff_time > (timeFraction*searchTime) )
			*trigger = true;
	}
}
