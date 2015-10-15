#include <bitset>
#include <vector>

const string white_bg = "\x1b[47m";
const string black_bg = "\x1b[40m";
const string white_text  = "\x1b[30m";
const string black_text  = "\x1b[37m";

Board::~Board() {
	delete white_pieces;
	delete white_kings;
	delete black_pieces;
	delete black_kings;
}

void Board::display() {
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
					if (white_kings->test(i)) {
						cout << "  " << white_bg << white_text << "K♔" << "\x1b[41m" << "  ";
					}
					else if (black_kings->test(i)) {
						cout << "  " << black_bg << black_text << "K♔" << "\x1b[41m" << "  ";
					}
					else if (white_pieces->test(i)) {
						cout << "  " << white_bg << white_text << "  " << "\x1b[41m" << "  ";
					}
					else if (black_pieces->test(i)) {
						cout << "  " << black_bg << black_text << "  " << "\x1b[41m" << "  ";
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

bool Board::no_piece(size_t i) {
	return !white_pieces->test(i) && !black_pieces->test(i);
}

bool Board::opponent_piece(Player p, size_t i) {
	return p == WHITE ? black_pieces->test(i) : white_pieces->test(i);
}

bool Board::white_piece(Player p, size_t i) {
	return (white_pieces->test(i) && turn == WHITE);
}

bool Board::black_piece(Player p, size_t i) {
	return (black_pieces->test(i) && turn == BLACK);
}

bool Board::white_king(Player p, size_t i) {
	return (white_kings->test(i)  && turn == WHITE);
}

bool Board::black_king(Player p, size_t i) {
	return (black_kings->test(i)  && turn == BLACK);
}

std::vector<Move*>* Board::jumps_from_square(Player turn, size_t i) {
	auto jumpVector = new std::vector<Move*>();
	oddrow = (((i - (i % 4))/4) % 2);
	posInRow = i % 4;
	if ( i < 24 && (white_piece(i) || black_king(i)) ) {
		if (posInRow < 3 && !oddrow && opponent_piece(i+5) && no_piece(i+9)) {
			jumpVector->push_back(new Move(turn, i, i+5, i+9));
		}
		if (posInRow > 0 && !oddrow && opponent_piece(i+4) && no_piece(i+7)) {
			jumpVector->push_back(new Move(turn, i, i+4, i+7));
		}
		if (posInRow < 3 && oddrow && opponent_piece(i+4) && no_piece(i+9)) {
			jumpVector->push_back(new Move(turn, i, i+4, i+9));
		}
		if (posInRow > 0 && oddrow && opponent_piece(i+3) && no_piece(i+7)) {
			jumpVector->push_back(new Move(turn, i, i+3, i+7));
		}
	}
	if ( i > 7 && (black_piece(i) || white_king(i)) ) {
		if (posInRow < 3 && !oddrow && oP->check(i-3) && no_piece(i-7)) {
			jumpVector->push_back(new Move(turn, i, i-3, i-7));
		}
		if (posInRow > 0 && !oddrow && oP->check(i-4) && no_piece(i-9)) {
			jumpVector->push_back(new Move(turn, i, i-4, i-9));
		}
		if (posInRow < 3 && oddrow && oP->check(i-4) && no_piece(i-7)) {
			jumpVector->push_back(new Move(turn, i, i-4, i-7));
		}
		if (posInRow > 0 && oddrow && oP->check(i-5) && no_piece(i-9)) {
			jumpVector->push_back(new Move(turn, i, i-5, i-9));
		}
	}
	return jumpVector;
}

std::vector<std::vector<Move*>*> Board::possible_moves(Player turn) {
	bool jumpFound = false;
	bool oddrow;
	size_t posInRow;

	auto jumps = std::vector<Move*>*();

	for (size_t i = 0; i < 32; i++) {
		auto j = jumps_from_square(Player turn)
		jumps.insert(jumps.end(), j.begin(), j.end());
		delete j;
	}
}