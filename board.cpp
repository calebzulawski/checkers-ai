#include <stdint.h>
#include <iostream>
#include "board.hpp"

Player::Player(bool white) {
	if (white) {
		initWhite();
	}
	else {
		initBlack();
	}
}

void Player::initWhite() {
	pieces = 0x00000FFF;
	kings = 0;
	white = true;
}

void Player::initBlack() {
	pieces = 0xFFF00000;
	kings = 0;
	white = false;
}

class Move {
public:
	int startIdx;
	int endIdx;
	int *nextMove = NULL;
};

void display_board(Player *p1, Player *p2) {
	string p1Color = p1->white ? "\x1b[37m" : "\x1b[30m";
	string p2Color = p2->white ? "\x1b[37m" : "\x1b[30m";
	string backgroundColor;
	bool oddrow;
	for (int i = 0; i < 32; i++) {
		oddrow = (((i - (i % 4))/4) % 2);
		if (!oddrow) {
			cout << "\x1b[43m" << " ";
		}
		cout << "\x1b[41m";
		if (p1->kings & (1 << i)) {
			cout << p1Color << "@";
		}
		else if (p2->kings & (1 << i)) {
			cout << p2Color << "@";
		}
		else if (p1->pieces & (1 << i)) {
			cout << p1Color << "O";
		}
		else if (p2->pieces & (1 << i)) {
			cout << p2Color << "O";
		}
		else {
			cout << " ";
		}
		if (oddrow) {
			cout << "\x1b[43m" << " ";
		}
		if (!((i+1) % 4))
			cout << "\x1b[0m" << " " << oddrow << endl;
	}
	cout << endl;
}