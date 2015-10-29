#include <bitset>
#include <vector>
#include <vector>
#include <string>
#include <iostream>
#include <limits>

#include "board.h"
#include "move.h"
#include "constants.h"

Board::~Board() {
	delete white_pieces;
	delete white_kings;
	delete black_pieces;
	delete black_kings;
}

void Board::clear() {
	white_pieces->reset();
	white_kings->reset();
	black_pieces->reset();
	black_kings->reset();
}

void Board::display() {
	// Draw top border
	std::cout << red_bg;
	for (int i = 0; i < 56; i++)
		std::cout << " ";
	std::cout << reset_scr << std::endl;
	std::cout << red_bg << "  ";
	std::cout << white_bg;
	for (int i = 0; i < 52; i++)
		std::cout << " ";
	std::cout << red_bg << "  " << reset_scr << std::endl;

	bool oddrow;
	for (int i1 = 0; i1 < 8; i1++) {
		for (int j = 0; j < 3; j++) {
			for (int i2 = 0; i2 < 4; i2++) {
				int i = i1*4 + i2;

				// Draw left border
				if (i2 == 0) {
					std::cout << red_bg << "  ";
					if (i1 < 4)
						std::cout << white_bg << "  ";
					else
						std::cout << black_bg << "  ";
				}

				// Draw board
				oddrow = (((i - (i % 4))/4) % 2);
				if (!oddrow) {
					std::cout << yellow_bg << "      ";
				}
				std::cout << red_bg;
				if (j == 0) {
					if (i < 10) {
						std::cout << i << "     ";
					} else {
						std::cout << i << "    ";
					}
				}
				else if (j == 1) {
					if (white_kings->test(i)) {
						std::cout << "  " << white_bg << white_text << "K " << red_bg << "  ";
					}
					else if (black_kings->test(i)) {
						std::cout << "  " << black_bg << black_text << "K " << red_bg << "  ";
					}
					else if (white_pieces->test(i)) {
						std::cout << "  " << white_bg << white_text << "  " << red_bg << "  ";
					}
					else if (black_pieces->test(i)) {
						std::cout << "  " << black_bg << black_text << "  " << red_bg << "  ";
					}
					else {
						std::cout << "      ";
					}
				}
				else if (j == 2) {
					std::cout << "      ";
				}
				if (oddrow) {
					std::cout << yellow_bg << "      ";
				}

				// Draw right border
				if (i2 == 3) {
					if (i1 < 4)
						std::cout << white_bg << "  ";
					else
						std::cout << black_bg << "  ";
					std::cout << red_bg << "  ";
				}

				if (!((i+1) % 4))
					std::cout << reset_scr << std::endl;
			}
		}
	}
	// Draw bottom border
	std::cout << red_bg << "  ";
	std::cout << black_bg;
	for (int i = 0; i < 52; i++)
		std::cout << " ";
	std::cout << red_bg << "  " << reset_scr << std::endl;
	std::cout << red_bg;
	for (int i = 0; i < 56; i++)
		std::cout << " ";
	std::cout << reset_scr << std::endl;
}

bool Board::no_piece(size_t i) {
	return !white_pieces->test(i) && !black_pieces->test(i);
}

bool Board::opponent_piece(Player p, size_t i) {
	return p == WHITE ? black_pieces->test(i) : white_pieces->test(i);
}

bool Board::is_white_piece(Player p, size_t i) {
	return (white_pieces->test(i) && p == WHITE);
}

bool Board::is_black_piece(Player p, size_t i) {
	return (black_pieces->test(i) && p == BLACK);
}

bool Board::is_white_king(Player p, size_t i) {
	return (white_kings->test(i)  && p == WHITE);
}

bool Board::is_black_king(Player p, size_t i) {
	return (black_kings->test(i)  && p == BLACK);
}

void Board::jumps_from_square(size_t i, Player p, std::vector<Move> &v) {
	bool oddrow = (((i - (i % 4))/4) % 2);
	size_t posInRow = i % 4;
	if ( i < 24 && (is_white_piece(p, i) || is_black_king(p, i)) ) {
		if (posInRow < 3 && !oddrow && opponent_piece(p, i+5) && no_piece(i+9))
			v.push_back(Move(p, this, i, i+5, i+9));
		if (posInRow > 0 && !oddrow && opponent_piece(p, i+4) && no_piece(i+7))
			v.push_back(Move(p, this, i, i+4, i+7));
		if (posInRow < 3 && oddrow && opponent_piece(p, i+4) && no_piece(i+9))
			v.push_back(Move(p, this, i, i+4, i+9));
		if (posInRow > 0 && oddrow && opponent_piece(p, i+3) && no_piece(i+7))
			v.push_back(Move(p, this, i, i+3, i+7));
	}
	if ( i > 7 && (is_black_piece(p, i) || is_white_king(p, i)) ) {
		if (posInRow < 3 && !oddrow && opponent_piece(p, i-3) && no_piece(i-7))
			v.push_back(Move(p, this, i, i-3, i-7));
		if (posInRow > 0 && !oddrow && opponent_piece(p, i-4) && no_piece(i-9))
			v.push_back(Move(p, this, i, i-4, i-9));
		if (posInRow < 3 && oddrow && opponent_piece(p, i-4) && no_piece(i-7))
			v.push_back(Move(p, this, i, i-4, i-7));
		if (posInRow > 0 && oddrow && opponent_piece(p, i-5) && no_piece(i-9))
			v.push_back(Move(p, this, i, i-5, i-9));
	}
	return;
}

void Board::shifts_from_square(size_t i, Player p, std::vector<Move> &v) {
	bool oddrow = (((i - (i % 4))/4) % 2);
	size_t posInRow = i % 4;
	if ( i < 28 && (is_white_piece(p, i) || is_black_king(p, i)) ) {
		if (posInRow < 3 && !oddrow && no_piece(i+5))
			v.push_back(std::move(Move(p, this, i, i+5)));
		if (!oddrow && no_piece(i+4))
			v.push_back(std::move(Move(p, this, i, i+4)));
		if (posInRow > 0 && oddrow && no_piece(i+3))
			v.push_back(std::move(Move(p, this, i, i+3)));
		if (oddrow && no_piece(i+4))
			v.push_back(std::move(Move(p, this, i, i+4)));
	}
	if ( i > 3 && (is_black_piece(p, i) || is_white_king(p, i)) ) {
		if (posInRow < 3 && !oddrow && no_piece(i-3))
			v.push_back(std::move(Move(p, this, i, i-3)));
		if (!oddrow && no_piece(i-4))
			v.push_back(std::move(Move(p, this, i, i-4)));
		if (posInRow > 0 && oddrow && no_piece(i-5))
			v.push_back(std::move(Move(p, this, i, i-5)));
		if (oddrow && no_piece(i-4))
			v.push_back(std::move(Move(p, this, i, i-4)));
	}
}

void Board::possible_moves(Player turn, std::vector<Move> &moves) {
	for (size_t i = 0; i < 32; i++)
		jumps_from_square(i, turn, moves);

	if (moves.size() == 0) {
		for (size_t i = 0; i < 32; i++)
			shifts_from_square(i, turn, moves);
	}
}

void Board::apply_move(Player turn, size_t start, size_t end) {
	if (turn == WHITE) {
		white_pieces->reset(start);
		white_pieces->set(end);
		if (end > 27)
			white_kings->set(end);
		if (white_kings->test(start)) {
			white_kings->reset(start);
			white_kings->set(end);
		}
	} else {
		black_pieces->reset(start);
		black_pieces->set(end);
		if (end < 4)
			black_kings->set(end);
		if (black_kings->test(start)) {
			black_kings->reset(start);
			black_kings->set(end);
		}
	}
}

void Board::apply_move(Player turn, size_t start, size_t jumped, size_t end) {
	apply_move(turn, start, end);
	if (turn == WHITE) {
		black_pieces->reset(jumped);
		black_kings->reset(jumped);
	} else {
		white_pieces->reset(jumped);
		white_kings->reset(jumped);
	}
}

/* ------------ */
/* AI FUNCTIONS */
/* ------------ */
void Board::alpha_beta_start(size_t depth, Player maximize, std::vector<Move> moves, Move &bestMove, bool &trigger) {
	float v = std::numeric_limits<float>::lowest();
	float alpha = std::numeric_limits<float>::lowest();
	float beta = std::numeric_limits<float>::max();
	for (auto move : moves) {
		float v_new = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(maximize), trigger);
		if (v_new > v) {
			v = v_new;
			bestMove = move;
		}
		alpha = std::max(alpha, v);
		if (beta <= alpha)
			break;
	}
	trigger = true;
}

float Board::alpha_beta(size_t depth, float alpha, float beta, Player maximize, Player current, bool &trigger) {
	if (depth == 0 || trigger)
		return score(maximize);
	
	std::vector<Move> moves;
	possible_moves(current, moves);


	if (moves.size() == 0)
		return score(maximize);

	if (current == maximize) {
		float v = std::numeric_limits<float>::lowest();
		for (auto move : moves) {
			float v_new = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(current), trigger);
			v = std::max(v, v_new);
			alpha = std::max(alpha, v);
			if (beta <= alpha)
				break;
		}
		return v;
	} else {
		float v = std::numeric_limits<float>::max();
		for (auto move : moves) {
			float v_new = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(current), trigger);
			v = std::min(v, v_new) ;
			beta = std::min(beta, v);
			if (beta <= alpha)
				break;
		}
		return v;
	}
}

float Board::score(Player p) {
	const float king_weight = 5;
	const float norm_weight = 3;
	float metric = king_weight * white_kings->count()
	             + norm_weight * white_pieces->count()
	             - king_weight * black_kings->count()
	             - norm_weight * black_pieces->count()
	             + (rand()%100)/1000.;
	return p == WHITE ? metric : -metric;
}