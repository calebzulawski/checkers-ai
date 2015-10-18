#include <limits>

#include "constants.h"

#include "board.h"
#include "move.h"
#include "ai.h"

float alpha_beta(Board *b, size_t depth, float alpha, float beta, Player maximize, Player current) {
	if (depth == 0)
		return score(maximize, b);
	
	std::vector<Move> moves;
	b->possible_moves(current, moves);

	if (current == maximize) {
		float v = std::numeric_limits::lowest<float>();
		for (auto move : moves) {
			float v_new = alpha_beta(move.board, depth-1, alpha, beta, maximize, other_player(current));
			v = std::max(v, v_new);
			alpha = std::max(alpha, v);
			if (beta <= alpha)
				break;
		}
		return v;
	} else {
		float v = std::numeric_limits::max<float>();
		for (auto move : moves) {
			float v_new = alpha_beta(move.board, depth-1, alpha, beta, maximize, other_player(current));
			v = std::min(v, v_new) ;
			beta = std::min(beta, v);
			if (beta <= alpha)
				break;
		}
		return v;
	}
}