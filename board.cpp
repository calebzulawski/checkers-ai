#include <stdint.h>
#include <iostream>
#include <vector>
#include "board.hpp"

#define CHECK(field,bit) ((field) & (1 << bit))

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

bool Player::checkWhite(int pos) {
	return (pieces & (1 << pos)) && (white || (kings & (1 << pos)));
}

bool Player::checkBlack(int pos) {
	return (pieces & (1 << pos)) && (!white || (kings & (1 << pos)));
}

bool Player::check(int pos) {
	return (pieces & (1 << pos));
}

Move::Move(int sIdx, int eIdx) {
	startIdx = sIdx;
	endIdx = eIdx;
}

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
		else if CHECK(p2->kings, i) {
			cout << p2Color << "@";
		}
		else if CHECK(p1->pieces, i) {
			cout << p1Color << "O";
		}
		else if CHECK(p2->pieces, i) {
			cout << p2Color << "O";
		}
		else {
			cout << " ";
		}
		if (oddrow) {
			cout << "\x1b[43m" << " ";
		}
		if (!((i+1) % 4))
			cout << "\x1b[0m" << endl;
	}
	cout << endl;
}

void possible_moves(Player *mP, Player *oP, vector<vector<Move*>* > *moveList) {
	int oddrow;
	int posInRow;
	for(int i=0; i < 32; i++) {
		oddrow = (((i - (i % 4))/4) % 2);
		posInRow = i % 4;
		if (mP->checkWhite(i)) {
			if (posInRow < 3 && !oddrow && !oP->check(i+5) && !mP->check(i+5) && i < 27) {
				auto newMove = new Move(i,i+5);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i + 5
			}
			if (!oddrow && !(mP->check(i+4)) && !(oP->check(i+4)) && i < 27) {
				auto newMove = new Move(i,i+4);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i + 4
			}
			if (posInRow > 0 && oddrow && !oP->check(i+3) && !mP->check(i+3) && i < 27) {
				auto newMove = new Move(i,i+3);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i + 3
			}
			if (oddrow && !mP->check(i+4) && !oP->check(i+4) && i < 27) {
				auto newMove = new Move(i,i+4);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i + 4
			}
			//TODO: Jumps
		}
		if (mP->checkBlack(i)) {
			if (posInRow < 3 && !oddrow && !oP->check(i-3) && !mP->check(i-3) && i > 3) {
				auto newMove = new Move(i,i-3);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i - 3
			}
			if (!oddrow && !mP->check(i-4) && !oP->check(i-4) && i > 3) {
				auto newMove = new Move(i,i-4);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i - 4
			}
			if (posInRow > 0 && oddrow && !oP->check(i-5) && !mP->check(i-5) && i > 3) {
				auto newMove = new Move(i,i-5);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i - 5
			}
			if (oddrow && !mP->check(i-4) && !oP->check(i-4) && i > 3) {
				auto newMove = new Move(i,i-4);
				moveList->push_back(new vector<Move*>(1,newMove));
				// i - 4
			}
			//TODO: Jumps
		}
	}
}