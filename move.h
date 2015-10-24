#ifndef MOVE_H
#define MOVE_H

class Move;

#include <vector>

#include "board.h"
#include "constants.h"

class Move {
public:
    Move();
    Move(const Move &m) :
    	turn (m.turn),
    	board(new Board(*m.board)),
    	start(m.start),
    	end  (m.end),
    	jump (m.jump),
        children(m.children)
    {};
    Move& operator=(const Move& m) {
        turn  = m.turn;
        board = new Board(*m.board);
        start = m.start;
        end   = m.end;
        jump  = m.jump;
        children = m.children;
        return *this;
    }
    Move(Player p, Board *b, size_t start, size_t end);
    Move(Player p, Board *b, size_t start, size_t jumped, size_t end);
    ~Move();

    Player  turn;
    Board*  board;
    size_t  start, end;
    bool    jump;
    std::vector<Move> children;
};


#endif /* MOVE_H */