#include "ai.hpp"
#include "board.hpp"
#include <limits>
#include <mutex>

int rate_move(Move *move) {
	int mPieces = (double) __builtin_popcountl(move->mPieces);
	int mKings = __builtin_popcountl(move->mKings);
	int oPieces = __builtin_popcountl(move->oPieces);
	int oKings = __builtin_popcountl(move->oKings);
	return ( mPieces + 2*mKings - oPieces - 2*oKings );
	// Later add stuff for making keeping back row filled desirable
}

int alphabeta(Player *mP, Player *oP, Move *move, int depth, int alpha, int beta, bool maximize) {
	// If depth == 0 or terminal node
	if (depth == 0 || move->mPieces == 0 || move->oPieces == 0) {
		return rate_move(move);
	}

	int v = maximize ? numeric_limits<int>::min() : numeric_limits<int>::max();

	auto moveList = new vector<vector<Move*>* >(0);

	if(maximize) {
		possible_moves(mP, oP, moveList, move);	
	} else {
		uint32_t tempPieces, tempKings;
		tempPieces = move->mPieces;
		tempKings = move->mKings;
		move->mPieces = move->oPieces;
		move->mKings = move->oKings;
		move->oPieces = tempPieces;
		move->oKings = tempKings;
		possible_moves(oP, mP, moveList, move);
	}
	
	for(uint i=0; i < moveList->size(); i++) {
		int v2 = alphabeta(mP, oP, (*moveList)[i]->back(), depth-1, alpha, beta, !maximize);
		if (maximize) {
			v = max(v, v2);
			alpha = max(alpha, v);
		} else {
			v = min(v, v2);
			beta = min(beta, v);
		}
		if (beta <= alpha)
			break;
	}
	delete moveList;
	return v;
}

void iterative_deepening(Player *mP, Player *oP, vector<Move*> **bestMove, mutex *bestMove_mutex, bool keepAlive) {
	// Max step and return best move
	int depth = 0;
	int besti = -1;
	while(keepAlive) {
		int alpha = numeric_limits<int>::min();
		int beta = numeric_limits<int>::max();

		int v = numeric_limits<int>::min();

		auto moveList = new vector<vector<Move*>* >(0);
		possible_moves(mP,oP,moveList,NULL);

		for(uint i=0; i < moveList->size(); i++) {
			int v2 = alphabeta(mP, oP, (*moveList)[i]->back(), depth, alpha, beta, false);
			if (v2 > v)
				besti = i;
			v = max(v, v2);
			alpha = max(alpha, v);
			if (beta <= alpha)
				break;
		}
		bestMove_mutex->lock();
		*bestMove = (*moveList)[besti];
		bestMove_mutex->unlock();
		delete moveList;
		depth++;
	}
}