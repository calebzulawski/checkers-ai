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
	Player(bool white);
};

void display_board(Player *p1, Player *p2);

#endif /* BOARD_H */