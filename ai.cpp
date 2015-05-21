#include "ai.hpp"
#include "board.hpp"

int rate_move(Move *move) {
	int mPieces = __builtin_popcountl(move->mPieces);
	int mKings = __builtin_popcountl(move->mKings);
	int oPieces = __builtin_popcountl(move->oPieces);
	int oKings = __builtin_popcountl(move->oKings);
	return ( mPieces + 2*mKings - oPieces - 2*oKings );
}