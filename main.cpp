#include "board.hpp"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));
	bool color = rand() % 2;
	auto ai = new Player(color);
	auto opp = new Player(!color);
	//display_board(ai,opp);
	auto moveList = new vector<vector<Move*>* >(0);
	int move;
	bool turn = false;
	Player *mP; // Moving player
	Player *oP; // Other player
	Player *wP; // White player
	Player *bP; // Black player
	if (ai->white) {
		wP = ai;
		bP = opp;
	} else {
		wP = opp;
		bP = ai;
	}
	while(true) {
		mP = turn ? wP : bP;
		oP = turn ? bP : wP;
		turn = !turn;

		moveList->clear();
		possible_moves(mP,oP,moveList);
		if (moveList->size() > 0) {
			move = rand() % moveList->size();
			mP->pieces = (*moveList)[move]->front()->mPieces;
			mP->kings = (*moveList)[move]->front()->mKings;
			oP->pieces = (*moveList)[move]->front()->oPieces;
			oP->kings = (*moveList)[move]->front()->oKings;
			display_board(ai,opp);
		} else {
			break;
		}
	}
}