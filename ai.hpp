#ifndef AI_H
#define AI_H

#include "board.hpp"

int rate_move(Move *move);

int alphabeta(Player *mP, Player *oP, Move *move, int depth, int alpha, int beta, bool maximize);

#endif /* AI_H */