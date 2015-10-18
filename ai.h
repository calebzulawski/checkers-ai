#include "board.h"
#include "constants.h"

static float alpha_beta(Board *b, size_t depth, float alpha, float beta, Player maximize, Player current);