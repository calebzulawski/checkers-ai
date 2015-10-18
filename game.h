#ifndef GAME_H
#define GAME_H

class Game;

#include "move.h"
#include "board.h"

class Game {
public:
    Game(bool _whiteIsAi, bool _blackIsAi) :
        whiteIsAi(_whiteIsAi),
        blackIsAi(_blackIsAi),
        board(new Board())
    {}

    void run();
    void get_command(std::vector<size_t> &command);
    bool isAI(Player turn) { return (turn == WHITE && whiteIsAi) || (turn == BLACK && blackIsAi); };

private:
    bool   whiteIsAi,
           blackIsAi;
    Board *board;

};

#endif /* GAME_H */