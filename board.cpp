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
	int offset;
	for (int i = 0; i < 64; i++) {
		offset = (i - (i % 8) + 1 / 8) % 2 ? 1 : 0;
		if ((i + offset) % 2) {
			cout << "\x1b[43m" << " ";
		}
		else {
			cout << "\x1b[41m";
			if (p1->kings & (1 << ((i - offset)/2))) {
				cout << p1Color << "k";
			}
			else if (p2->kings & (1 << ((i - offset)/2))) {
				cout << p2Color << "k";
			}
			else if (p1->pieces & (1 << ((i - offset)/2))) {
				cout << p1Color << "1";
			}
			else if (p2->pieces & (1 << ((i - offset)/2))) {
				cout << p2Color << "2";
			}
			else {
				cout << " ";
			}
		}
		if (!((i+1) % 8))
			cout << "\x1b[0m" << endl;
	}
	cout << endl;
}