#include "move.h"
#include "board.h"

#include "iostream"

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
	if (!board->apply_move(turn, start, jumped_, end))
		board->jumps_from_square(end, turn, children);
}

void Move::display() {
	std::cout << start << " -> " << end;
	for (auto value : path)
		std::cout << " -> " << value;
	std::cout << std::endl;
}

void split_moves(std::vector<Move> &root, bool tracePaths) {
	std::vector<Move>   added;
	std::vector<size_t> path;
	for (auto it =  root.begin(); it != root.end();) {
		if (it->children.size() != 0) {
			split_moves_recur(added, it->children, (tracePaths ? &path : nullptr), it->start, it->end);
			it = root.erase(it);
		} else {
			++it;
		}
	}
	root.insert(root.end(), added.begin(), added.end());
}

void split_moves_recur(std::vector<Move> &root, std::vector<Move> &v, std::vector<size_t> *path /* = nullptr */, size_t start /* = 32 */, size_t end /* = 32 */) {
	for (auto it =  v.begin(); it != v.end(); ++it) {
		if (path) path->push_back(it->end);
		if (it->children.size() != 0) {
			split_moves_recur(root, it->children, path, start, end);
		} else {
			root.push_back(*it);
			if (path) {
				root.rbegin()->path.assign(path->begin(), path->end());
				root.rbegin()->start = start;
				root.rbegin()->end   = end;
			}
		}
		if (path) path->erase(path->end()-1);
	}
}