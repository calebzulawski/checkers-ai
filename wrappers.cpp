#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include "board.hpp"
#include "ai.hpp"
#include "wrappers.hpp"

Move* PlayerWrapper::get_move(Player *opponent) {
	cout << (white ? "White" : "Black") << " - " << (human ? "Human" : "Computer") << " turn" << endl;
	if (human) {
		while(true) {
			auto moveList = new vector<vector<Move*>* >(0);
			possible_moves(player,opponent,moveList,NULL);
			for (uint i=0; i < moveList->size(); i++) {
				for (uint j=0; j < (*moveList)[i]->size(); j++) {
					if (j == 0) {
						cout << (*moveList)[i]->front()->startIdx;
					}
					cout << " --> " << (*(*moveList)[i])[j]->endIdx;
				}
				cout << endl;
			}
			
			if (moveList->size() == 0)
				return NULL;

			// Read input command
			cout << "Enter move: ";
			string s;
			getline(cin,s);
			stringstream ss(s);
			int temp;
			vector<int> command(0);
			while(ss >> temp)
				command.push_back(temp);

			for (uint i=0; i < moveList->size(); i++) {
				if ((command.size() - 1) == (*moveList)[i]->size() && command.front() == (*moveList)[i]->front()->startIdx) {
					bool good = true;
					for (uint j=1; j < command.size(); j++) {
						if (command[j] != (*(*moveList)[i])[j-1]->endIdx)
							good = false;
					}
					if (good == true)
						return (*moveList)[i]->back();
				}
			}
		}
	} else {
		auto bestMove = new vector<Move*>(0);
		auto bestMove_mutex = new mutex;
		auto keepAlive = new bool(true);
		thread search (iterative_deepening, player, opponent, bestMove, bestMove_mutex, keepAlive);
		this_thread::sleep_for(std::chrono::seconds(5));
		*keepAlive = false;
		search.detach();
		return bestMove->back();
	}
}

PlayerWrapper::PlayerWrapper(bool whiteArg, bool humanArg) {
	white = whiteArg;
	human = humanArg;
	player = new Player(white);
}

void PlayerWrapper::update(Move *move, bool active) {
	player->pieces = (active ? move->mPieces : move->oPieces);
	player->kings = (active ? move->mKings : move->oKings);
}

GameWrapper::GameWrapper(bool blackAI, bool whiteAI) {
	blackPlayer = new PlayerWrapper(false, blackAI);
	whitePlayer = new PlayerWrapper(true, whiteAI);
}

void GameWrapper::evaluate_turn() {
	Move *nextMove;
	if (blackTurn) {
		nextMove = blackPlayer->get_move(whitePlayer->player);
		if (nextMove) {
			blackPlayer->update(nextMove, true);
			whitePlayer->update(nextMove, false);
		} else {
			cout << "White wins!" << endl;
			done = true;
		}
	} else {
		nextMove = whitePlayer->get_move(blackPlayer->player);
		if (nextMove) {
			whitePlayer->update(nextMove, true);
			blackPlayer->update(nextMove, false);
		} else {
			cout << "Black wins!" << endl;
			done = true;
		}
	}
	blackTurn = !blackTurn;
}

void GameWrapper::run() {
	display_board(whitePlayer->player, blackPlayer->player);
	while(!done) {
		evaluate_turn();
		display_board(whitePlayer->player, blackPlayer->player);
	}
}