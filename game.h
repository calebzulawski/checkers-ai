#ifndef GAME_H
#define GAME_H

#include <bitset>
#include <vector>

class Move {
public:
    Board* board;
    size_t start, end;
};

class Board {
public:
    Board() :
        white_pieces( new std::bitset<32> (0x00000FFF) ),
        white_kings ( new std::bitset<32> (0) ),
        black_pieces( new std::bitset<32> (0xFFF00000) ),
        black_kings ( new std::bitset<32> (0) )
    {};
    Board(Board* b) :
        white_pieces( new std::bitset<32> (b->white_pieces.to_ulong()) ),
        white_kings ( new std::bitset<32> (b->white_kings.to_ulong() ) ),
        black_pieces( new std::bitset<32> (b->black_pieces.to_ulong()) ),
        black_kings ( new std::bitset<32> (b->black_kings.to_ulong() ) )
    {};
    ~Board();

    void display();

    // Functions for cleaning up rules
    bool no_piece(size_t i);
    bool is_piece(Player p, size_t i);
    bool white_piece(Player p, size_t i);
    bool black_piece(Player p, size_t i);
    bool white_king (Player p, size_t i);
    bool black_king (Player p, size_t i);

    std::vector<Move>* possible_moves(Player turn);

private:
    std::bitset<32> *white_pieces,
                    *black_pieces,
                    *white_kings,
                    *black_kings;

};

class Game {
public:
    Game(bool _whiteIsAi, bool _blackIsAi) :
        whiteIsAi(_whiteIsAi),
        blackIsAi(_blackIsAi),
        board(new Board())
    {}

    void run();

private:
    bool   whiteIsAi,
           blackIsAi;
    Board *board;

};

enum Player {WHITE, BLACK};

#endif /* GAME_H */