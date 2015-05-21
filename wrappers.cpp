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
		return (*moveList)[rand() % moveList->size()]->back();
	}
}

