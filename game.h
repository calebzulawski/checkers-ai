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

    Game() :
        board(new Board())
    {}

    void load(char *filename);
    void prompt();
    void list_moves(std::vector<Move> &v);
    void run();
    bool isAI(Player turn) { return (turn == WHITE && whiteIsAi) || (turn == BLACK && blackIsAi); };

    void takeTurn_AI(std::vector<Move> &moves);
    void takeTurn_player(std::vector<Move> &moves);

    bool   whiteIsAi,
           blackIsAi;
    float  searchTime = -1;
    Board *board;
    Player turn = WHITE;

};

void timer(bool *trigger, float time);

#endif /* GAME_H */