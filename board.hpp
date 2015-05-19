#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <iostream>

using namespace std;

class Player {
public:
	uint32_t pieces;
	uint32_t kings;
	bool white;
	void initWhite();
	void initBlack();
	bool checkWhite(int pos);
	bool checkBlack(int pos);
	bool check(int pos);
	Player(bool white);
};

void display_board(Player *p1, Player *p2);

void possible_moves(Player *mP, Player *oP, int *moveList);

#endif /* BOARD_H */