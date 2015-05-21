#include <iostream>
#include "board.hpp"
#include "ai.hpp"
#include "wrappers.hpp"

Move* PlayerWrapper::get_move(Player *opponent) {
	cout << (white ? "White" : "Black") << " - " << (human ? "Human" : "Computer") << " turn";
	if (human) {
		while(true) {
			auto moveList = new vector<vector<Move*>* >(0);
			possible_moves(player,opponent,moveList,NULL);
			
			if (moveList->size() == 0)
				return NULL;
			
			int startIdx;
			int endIdx;
			cout << "Select piece: ";
			cin >> startIdx;
			cout << "Move to: ";
			cin >> endIdx;

			bool multijump = false;
			// Check if valid move
			for(uint i=0; i < moveList->size(); i++) {
				if ((*moveList)[i]->front()->startIdx == startIdx && (*moveList)[i]->front()->endIdx) {
					if ((*moveList)[i]->size() == 1) {
						// Works but memory leak
						return (*moveList)[i]->front();
					} else {
						multijump = true;
					}
				}
			}
			int jumps [32];
			uint idx = 0;
			while(multijump) {
				cout << "Continue jump to: ";
				cin >> jumps[idx];
				multijump = false;
				for(uint i=0; i < moveList->size(); i++) {
					if ((*moveList)[i]->front()->startIdx == startIdx && (*moveList)[i]->front()->endIdx && (*moveList)[i]->size() >= (idx + 1)) {
						bool correct = true;
						for(uint j=1; j < (*moveList)[i]->size(); j++) {
							if ((*(*moveList)[i])[j]->endIdx != jumps[j-1]) {
								correct = false;
							}
						}
						if (correct) {
							// Works but memory leak
							if ((*moveList)[i]->size() == (idx + 1)) {
								return (*moveList)[i]->back();
							} else {
								multijump = true;
							}
						}
					}
				}
			}
		}
	} else {
		auto moveList = new vector<vector<Move*>* >(0);
		possible_moves(player,opponent,moveList,NULL);
		if (moveList->size() == 0)
			return NULL;
		return (*moveList)[rand() % moveList->size()]->back();
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
	Move *newMove;
	if (blackTurn) {
		nextMove = blackPlayer->get_move(whitePlayer->player);
		if (nextMove) {
			blackPlayer->update(nextMove, true);
			whitePlayer->update(nextMove, false);
		} else {
			cout << "White wins!" << endl;
			exit(0);
		}
	} else {
		nextMove = whitePlayer->get_move(blackPlayer->player);
		if (nextMove) {
			whitePlayer->update(nextMove, true);
			blackPlayer->update(nextMove, false);
		} else {
			cout << "Black wins!" << endl;
			exit(0);
		}
	}
}