#include "move.h"
#include "board.h"

Move::Move() :
	board(new Board())
{}

Move::~Move() {
	delete board;
}

Move::Move(Player p, Board *b, size_t start_, size_t end_) :
	turn(p),
	board(new Board(*b)),
	start(start_),
	end(end_),
	jump(false)
{
	board->apply_move(turn, start, end);
}

Move::Move(Player p, Board *b, size_t start_, size_t jumped_, size_t end_) :
	turn(p),
	board(new Board(*b)),
	start(start_),
	end(end_),
	jump(true)
{
	board->apply_move(turn, start, jumped_, end);
	board->jumps_from_square(end, turn, children);
}