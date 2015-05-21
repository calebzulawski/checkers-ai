#include <stdint.h>
#include <stdlib.h>
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

Move::Move(int sIdx, int eIdx, Player *mP, Player *oP) {
	startIdx = sIdx;
	endIdx = eIdx;
	mPieces = mP->pieces;
	mKings = mP->kings;
	oPieces = oP->pieces;
	oKings = oP->kings;

	// Set new bits
	mPieces |= 1 << endIdx;
	// Check and set if king
	mKings |= ((mKings >> startIdx) & 1) << endIdx;
	// Clear old bits
	mPieces &= ~(1 << startIdx);
	mKings &= ~(1 << startIdx);

	// King me
	mKings |= (mP->white ? 0xF0000000 : 0x0000000F) & mPieces;
}

void Move::jump(int idx) {
	// Clear jumped bits
	oPieces &= ~(1 << idx);
	oKings &= ~(1 << idx);
}

void display_board(Player *p1, Player *p2) {
	string p1Color = p1->white ? "\x1b[47m" : "\x1b[40m";
	string p2Color = p2->white ? "\x1b[47m" : "\x1b[40m";
	string p1TextColor = p1->white ? "\x1b[30m" : "\x1b[37m";
	string p2TextColor = p1->white ? "\x1b[30m" : "\x1b[37m";
	string backgroundColor;
	bool oddrow;
	for (int i1 = 0; i1 < 8; i1++) {
		for (int j = 0; j < 3; j++) {
			for (int i2 = 0; i2 < 4; i2++) {
				int i = i1*4 + i2;
				oddrow = (((i - (i % 4))/4) % 2);
				if (!oddrow) {
					cout << "\x1b[43m" << "      ";
				}
				cout << "\x1b[41m";
				if (j == 0) {
					if (i < 10) {
						cout << i << "     ";
					} else {
						cout << i << "    ";
					}
				}
				else if (j == 1) {
					if (p1->kings & (1 << i)) {
						cout << "  " << p1Color << p1TextColor << "K♔" << "\x1b[41m" << "  ";
					}
					else if CHECK(p2->kings, i) {
						cout << "  " << p2Color << p2TextColor << "K♔" << "\x1b[41m" << "  ";
					}
					else if CHECK(p1->pieces, i) {
						cout << "  " << p1Color << p1TextColor << "  " << "\x1b[41m" << "  ";
					}
					else if CHECK(p2->pieces, i) {
						cout << "  " << p2Color << p2TextColor << "  " << "\x1b[41m" << "  ";
					}
					else {
						cout << "      ";
					}
				}
				else if (j == 2) {
					cout << "      ";
				}
				if (oddrow) {
					cout << "\x1b[43m" << "      ";
				}
				if (!((i+1) % 4))
					cout << "\x1b[0m" << endl;
			}
		}
		//cout << endl;
	}
}

void possible_moves(Player *mPin, Player *oPin, vector<vector<Move*>* > *moveList, Move *currentMove) {
	auto mP = new Player(mPin->white);
	auto oP = new Player(oPin->white);
	if (currentMove) {
		mP->pieces = currentMove->mPieces;
		mP->kings = currentMove->mKings;
		oP->pieces = currentMove->oPieces;
		oP->kings = currentMove->oKings;
	} else {
		mP->pieces = mPin->pieces;
		mP->kings = mPin->kings;
		oP->pieces = oPin->pieces;
		oP->kings = oPin->kings;
	}

	int oddrow;
	int posInRow;

	movelist_gc(moveList);
	possible_jumps(mP, oP, moveList);

	if (moveList->size() == 0) {
		for(int i=0; i < 32; i++) {
			oddrow = (((i - (i % 4))/4) % 2);
			posInRow = i % 4;

			// Non-jump moves
			if (mP->checkWhite(i)) {
				if (posInRow < 3 && !oddrow && !oP->check(i+5) && !mP->check(i+5) && i < 28) {
					auto newMove = new Move(i,i+5,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (!oddrow && !mP->check(i+4) && !oP->check(i+4) && i < 28) {
					auto newMove = new Move(i,i+4,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (posInRow > 0 && oddrow && !oP->check(i+3) && !mP->check(i+3) && i < 28) {
					auto newMove = new Move(i,i+3,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (oddrow && !mP->check(i+4) && !oP->check(i+4) && i < 28) {
					auto newMove = new Move(i,i+4,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
			}
			if (mP->checkBlack(i)) {
				if (posInRow < 3 && !oddrow && !oP->check(i-3) && !mP->check(i-3) && i > 3) {
					auto newMove = new Move(i,i-3,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (!oddrow && !mP->check(i-4) && !oP->check(i-4) && i > 3) {
					auto newMove = new Move(i,i-4,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (posInRow > 0 && oddrow && !oP->check(i-5) && !mP->check(i-5) && i > 3) {
					auto newMove = new Move(i,i-5,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
				if (oddrow && !mP->check(i-4) && !oP->check(i-4) && i > 3) {
					auto newMove = new Move(i,i-4,mP,oP);
					moveList->push_back(new vector<Move*>(1,newMove));
				}
			}
		}
	}
	delete mP;
	delete oP;
}

void possible_jumps(Player *mP, Player *oP, vector<vector<Move*>* > *moveList) {
	auto jumpVector = new vector<Move*>(0);

	// Do jump finding stuff and add to "currentMove" vector (or create in case of NULL pointer)
	int oddrow;
	int posInRow;
	for(int i=0; i < 32; i++) {
		oddrow = (((i - (i % 4))/4) % 2);
		posInRow = i % 4;

		// Check for jumps
		if (mP->checkWhite(i)) {
			if (posInRow < 3 && !oddrow && oP->check(i+5) && !oP->check(i+9) && !mP->check(i+9) && i < 24) {
				auto newMove = new Move(i,i+9,mP,oP);
				newMove->jump(i+5);
				jumpVector->push_back(newMove);
			}
			if (posInRow > 0 && !oddrow && oP->check(i+4) && !mP->check(i+7) && !oP->check(i+7) && i < 24) {
				auto newMove = new Move(i,i+7,mP,oP);
				newMove->jump(i+4);
				jumpVector->push_back(newMove);
			}
			if (posInRow < 3 && oddrow && oP->check(i+4) && !oP->check(i+9) && !mP->check(i+9) && i < 24) {
				auto newMove = new Move(i,i+9,mP,oP);
				newMove->jump(i+4);
				jumpVector->push_back(newMove);
			}
			if (posInRow > 0 && oddrow && oP->check(i+3) && !mP->check(i+7) && !oP->check(i+7) && i < 24) {
				auto newMove = new Move(i,i+7,mP,oP);
				newMove->jump(i+3);
				jumpVector->push_back(newMove);
			}
		}
		if (mP->checkBlack(i)) {
			if (posInRow < 3 && !oddrow && oP->check(i-3) && !oP->check(i-7) && !mP->check(i-7) && i > 7) {
				auto newMove = new Move(i,i-7,mP,oP);
				newMove->jump(i-3);
				jumpVector->push_back(newMove);
			}
			if (posInRow > 0 && !oddrow && oP->check(i-4) && !mP->check(i-9) && !oP->check(i-9) && i > 7) {
				auto newMove = new Move(i,i-9,mP,oP);
				newMove->jump(i-4);
				jumpVector->push_back(newMove);
			}
			if (posInRow < 3 && oddrow && oP->check(i-4) && !oP->check(i-7) && !mP->check(i-7) && i > 7) {
				auto newMove = new Move(i,i-7,mP,oP);
				newMove->jump(i-4);
				jumpVector->push_back(newMove);
			}
			if (posInRow > 0 && oddrow && oP->check(i-5) && !mP->check(i-9) && !oP->check(i-9) && i > 7) {
				auto newMove = new Move(i,i-9,mP,oP);
				newMove->jump(i-5);
				jumpVector->push_back(newMove);
			}
		}
	}

	for(uint i=0; i < jumpVector->size(); i++) {
		auto tempMoveVec = new vector<Move*>(1,(*jumpVector)[i]);
		multiple_jumps(mP, oP, moveList,tempMoveVec);
	}

	delete jumpVector;
}

void multiple_jumps(Player *mPin, Player *oPin, vector<vector<Move*>* > *moveList, vector<Move*> *currentMove) {
	auto mP = new Player(mPin->white);
	auto oP = new Player(oPin->white);
	mP->pieces = currentMove->back()->mPieces;
	mP->kings = currentMove->back()->mKings;
	oP->pieces = currentMove->back()->oPieces;
	oP->kings = currentMove->back()->oKings;

	auto jumpVector = new vector<Move*>(0);

	// Check only piece that just jumped!
	int i = currentMove->back()->endIdx;
	int oddrow = (((i - (i % 4))/4) % 2);
	int posInRow = i % 4;

	// Non-jump moves
	if (mP->checkWhite(i)) {
		if (posInRow < 3 && !oddrow && oP->check(i+5) && !oP->check(i+9) && !mP->check(i+9) && i < 24) {
			auto newMove = new Move(i,i+9,mP,oP);
			newMove->jump(i+5);
			jumpVector->push_back(newMove);
		}
		if (posInRow > 0 && !oddrow && oP->check(i+4) && !mP->check(i+7) && !oP->check(i+7) && i < 24) {
			auto newMove = new Move(i,i+7,mP,oP);
			newMove->jump(i+4);
			jumpVector->push_back(newMove);
		}
		if (posInRow < 3 && oddrow && oP->check(i+4) && !oP->check(i+9) && !mP->check(i+9) && i < 24) {
			auto newMove = new Move(i,i+9,mP,oP);
			newMove->jump(i+4);
			jumpVector->push_back(newMove);
		}
		if (posInRow > 0 && oddrow && oP->check(i+3) && !mP->check(i+7) && !oP->check(i+7) && i < 24) {
			auto newMove = new Move(i,i+7,mP,oP);
			newMove->jump(i+3);
			jumpVector->push_back(newMove);
		}
	}
	if (mP->checkBlack(i)) {
		if (posInRow < 3 && !oddrow && oP->check(i-3) && !oP->check(i-7) && !mP->check(i-7) && i > 7) {
			auto newMove = new Move(i,i-7,mP,oP);
			newMove->jump(i-3);
			jumpVector->push_back(newMove);
		}
		if (posInRow > 0 && !oddrow && oP->check(i-4) && !mP->check(i-9) && !oP->check(i-9) && i > 7) {
			auto newMove = new Move(i,i-9,mP,oP);
			newMove->jump(i-4);
			jumpVector->push_back(newMove);
		}
		if (posInRow < 3 && oddrow && oP->check(i-4) && !oP->check(i-7) && !mP->check(i-7) && i > 7) {
			auto newMove = new Move(i,i-7,mP,oP);
			newMove->jump(i-4);
			jumpVector->push_back(newMove);
		}
		if (posInRow > 0 && oddrow && oP->check(i-5) && !mP->check(i-9) && !oP->check(i-9) && i > 7) {
			auto newMove = new Move(i,i-9,mP,oP);
			newMove->jump(i-5);
			jumpVector->push_back(newMove);
		}
	}

	if (jumpVector->size() == 0) {
		moveList->push_back(currentMove);
	} else {
		for (uint i=0; i < jumpVector->size(); i++) {
			auto tempMoveVec = new vector<Move*>(currentMove->size());
			for(uint j=0; j < tempMoveVec->size(); j++) {
				(*tempMoveVec)[j] = (*currentMove)[j]; 
			}
			tempMoveVec->push_back((*jumpVector)[i]);
			multiple_jumps(mP, oP, moveList,tempMoveVec);
		}
		delete currentMove;
	}

	delete jumpVector;
}

void movelist_gc(vector<vector<Move*>* > *moveList) {
	for(uint i=0; i < moveList->size(); i++) {
		for(uint j=0; j < (*moveList)[i]->size(); j++) {
			delete (*(*moveList)[i])[j];
		}
		delete (*moveList)[i];
	}
	moveList->clear();
}