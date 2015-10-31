#include <cstdlib>
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
					if ((*white_kings)[i]) {
						std::cout << "  " << white_bg << white_text << "K " << red_bg << "  ";
					}
					else if ((*black_kings)[i]) {
						std::cout << "  " << black_bg << black_text << "K " << red_bg << "  ";
					}
					else if ((*white_pieces)[i]) {
						std::cout << "  " << white_bg << white_text << "  " << red_bg << "  ";
					}
					else if ((*black_pieces)[i]) {
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
	return !(*white_pieces)[i] && !(*black_pieces)[i];
}

bool Board::opponent_piece(Player p, size_t i) {
	return p == WHITE ? (*black_pieces)[i] : (*white_pieces)[i];
}

bool Board::is_white_piece(Player p, size_t i) {
	return (p == WHITE && (*white_pieces)[i]);
}

bool Board::is_black_piece(Player p, size_t i) {
	return (p == BLACK && (*black_pieces)[i]);
}

bool Board::is_white_king(Player p, size_t i) {
	return (p == WHITE && (*white_kings)[i]);
}

bool Board::is_black_king(Player p, size_t i) {
	return (p == BLACK && (*black_kings)[i]);
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
		(*white_pieces)[start] = 0;
		(*white_pieces)[end]   = 1;
		if (end > 27)
			(*white_kings)[end] = 1;
		if ((*white_kings)[start]) {
			(*white_kings)[start] = 0;
			(*white_kings)[end]   = 1;
		}
	} else {
		(*black_pieces)[start] = 0;
		(*black_pieces)[end]   = 1;
		if (end < 4)
			(*black_kings)[end] = 1;
		if ((*black_kings)[start]) {
			(*black_kings)[start] = 0;
			(*black_kings)[end]   = 1;
		}
	}
}

void Board::apply_move(Player turn, size_t start, size_t jumped, size_t end) {
	apply_move(turn, start, end);
	if (turn == WHITE) {
		(*black_pieces)[jumped] = 0;
		(*black_kings )[jumped] = 0;
	} else {
		(*white_pieces)[jumped] = 0;
		(*white_kings )[jumped] = 0;
	}
}

/* ------------ */
/* AI FUNCTIONS */
/* ------------ */
size_t Board::iterative_deepening(Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger) {
	size_t depth = 1;
	size_t finished = 0;
	Move m;
	while (!trigger) {
		std::cout << "-----------------" << std::endl;
		alpha_beta_start(depth, maximize, moves, m, trigger);
		// negamax_start(depth, maximize, moves, m, trigger);
		// std::cin.get();
		if (!trigger) {
			bestMove = m;
			finished = depth;
		}
		depth++;
	}
	return finished;
}

void Board::negamax_start(size_t depth, Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger) {
	float v = std::numeric_limits<float>::lowest();
	float alpha = std::numeric_limits<float>::lowest();
	float beta = std::numeric_limits<float>::max();

	for (auto move : moves) {
		float v_new = -move.board->negamax(depth-1, -beta, -alpha, other_player(maximize), trigger);
		// for (int i = 0; i < depth; i++)
		// 	std::cout << "  ";
		// std::cout << v_new << std::endl;
		if (v_new > v) {
			v = v_new;
			bestMove = move;
		}
		alpha = std::max(alpha, v);
		if (beta <= alpha)
			break;
	}
}

float Board::negamax(size_t depth, float alpha, float beta, Player p, bool &trigger) {
	if (depth == 0 || trigger)
		return score(p);
	
	std::vector<Move> moves;
	possible_moves(p, moves);


	if (moves.size() == 0)
		return score(p);

	float v = std::numeric_limits<float>::lowest();
	for (auto move : moves) {
		float v_new = -move.board->negamax(depth-1, -beta, -alpha, other_player(p), trigger);
		// for (int i = 0; i < depth; i++)
			// std::cout << "  ";
		// std::cout << v_new << std::endl;
		v = std::max(v, v_new);
		alpha = std::max(alpha, v);
		if (beta <= alpha)
			break;
	}

	return v;
}

void Board::alpha_beta_start(size_t depth, Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger) {
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
		// float v = alpha;
		for (auto move : moves) {
			float v_new = move.board->alpha_beta(depth-1, v, beta, maximize, other_player(current), trigger);
			v = std::max(v, v_new);
			alpha = std::max(alpha, v);
			if (beta <= alpha)
				break;
		}
		return v;
	} else {
		float v = std::numeric_limits<float>::max();
		// float v = beta;
		for (auto move : moves) {
			float v_new = move.board->alpha_beta(depth-1, alpha, v, maximize, other_player(current), trigger);
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
	float metric = (king_weight - norm_weight) * white_kings->count()
	             - (king_weight - norm_weight) * black_kings->count()
	             + norm_weight * white_pieces->count()
	             - norm_weight * black_pieces->count()
	             + (rand()%100)/1000.;
	return p == WHITE ? metric : -metric;
}
