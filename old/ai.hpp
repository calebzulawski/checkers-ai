#ifndef AI_H
#define AI_H

#include "board.hpp"
#include <mutex>

int rate_move(Move *move);

int alphabeta(Player *mP, Player *oP, Move *move, int depth, int alpha, int beta, bool maximize);

void iterative_deepening(Player *mP, Player *oP, vector<Move*>* bestMove, mutex *bestMove_mutex, bool *keepAlive);

#endif /* AI_H */