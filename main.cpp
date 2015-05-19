#include "board.hpp"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
	bool color = rand() % 2;
	auto ai = new Player(color);
	auto opp = new Player(color);
	display_board(ai,opp);
	auto moveList = new vector<vector<Move*>* >(0);

	do {
		possible_moves(opp,ai,moveList);
		for (int i = 0; i < moveList->size(); i++) {
			cout << (*moveList)[i]->front()->startIdx << "\t";
			cout << (*moveList)[i]->front()->endIdx << endl;
		}
	} while (moveList->size() > 0)
}