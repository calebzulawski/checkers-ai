#include "move.h"
#include "board.h"

#include <map>

Move::Move() :
	board(nullptr)
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

void split_moves(std::vector<Move> &root, std::vector<Move> &v, std::vector<size_t> *path /* = nullptr */, bool top /* = true */, size_t start /* = 32 */, size_t end /* = 32 */) {
	for (auto it =  v.begin(); it != v.end(); ++it) {
		if (it->children.size() != 0) {
			if (path) path->push_back(it->end);
			if (top) {
				start = it->start;
				end = it->end;
			}
			split_moves(root, it->children, path, false, start, end);
			if (path) path->erase(path->end());
			if (top) it = v.erase(it);
		} else if (!top) {
			root.push_back(*it);
			root.end()->path.insert(root.end()->path.begin(), path->begin(), path->end());
			root.end()->start = start;
			root.end()->end   = end;
		}
	}
}