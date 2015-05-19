#include "board.hpp"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
	auto ai = new Player(true);
	auto opp = new Player(false);
	display_board(ai,opp);
	auto moveList = new vector<vector<Move*>* >(0);
	possible_moves(ai,opp,moveList);
	for (int i = 0; i < moveList->size(); i++) {
		cout << i << '\t' << moveList[i][0]->front()->endIdx << endl;
	}
}