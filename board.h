#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

//Forward declaration
class Move;

#include <bitset>
#include <vector>

#include "constants.h"

class Board {
public:
    Board() :
        white_pieces( new std::bitset<32> (0x00000FFF) ),
        black_pieces( new std::bitset<32> (0xFFF00000) ),
        white_kings ( new std::bitset<32> (0) ),
        black_kings ( new std::bitset<32> (0) )
    {};
    Board(const Board& b) :
        white_pieces( new std::bitset<32> (*b.white_pieces) ),
        black_pieces( new std::bitset<32> (*b.black_pieces) ),
        white_kings ( new std::bitset<32> (*b.white_kings ) ),
        black_kings ( new std::bitset<32> (*b.black_kings ) )
    {};
    Board& operator=(const Board& b) {
        delete white_pieces;
        delete black_pieces;
        delete white_kings;
        delete black_kings;
        white_pieces = new std::bitset<32> (*b.white_pieces);
        black_pieces = new std::bitset<32> (*b.black_pieces);
        white_kings  = new std::bitset<32> (*b.white_kings );
        black_kings  = new std::bitset<32> (*b.black_kings );
        return *this;
    }
    ~Board();

    void display();
    void clear();

    // Functions for determining various board states
    bool no_piece(size_t i);
    bool is_piece(Player p, size_t i);
    bool opponent_piece(Player p, size_t i);
    bool is_white_piece(Player p, size_t i);
    bool is_black_piece(Player p, size_t i);
    bool is_white_king (Player p, size_t i);
    bool is_black_king (Player p, size_t i);

    // Find moves
    void possible_moves(Player turn, std::vector<Move> &moves);
    void jumps_from_square(size_t i, Player p, std::vector<Move> &v);
    void shifts_from_square(size_t i, Player p, std::vector<Move> &v);
    bool apply_move(Player turn, size_t start, size_t end);
    bool apply_move(Player turn, size_t start, size_t jumped, size_t end);

    // AI
    size_t  iterative_deepening(Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger);
    void    alpha_beta_start(size_t depth, Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger);
    int64_t alpha_beta(size_t depth, int64_t alpha, int64_t beta, Player maximize, Player current, bool &trigger);
    int64_t score(Player p);
    inline int64_t score_0();
    inline int64_t score_1();
    inline int64_t score_2();

    std::bitset<32> *white_pieces,
                    *black_pieces,
                    *white_kings,
                    *black_kings;

private:
};

#endif /* BOARD_H */