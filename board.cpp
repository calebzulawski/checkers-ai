#include <cstdlib>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cstdint>

#include "board.h"
#include "move.h"
#include "constants.h"

constexpr int Board::endgame_weights[];

Board::~Board() {
    delete white_pieces;
    delete white_kings;
    delete black_pieces;
    delete black_kings;
}

void Board::clear() {
    white_pieces->reset();
    white_kings->reset();
    black_pieces->reset();
    black_kings->reset();
}

void Board::display() {
    // Draw top border
    std::cout << red_bg;
    for (int i = 0; i < 56; i++)
        std::cout << " ";
    std::cout << reset_scr << std::endl;
    std::cout << red_bg << "  ";
    std::cout << white_bg;
    for (int i = 0; i < 52; i++)
        std::cout << " ";
    std::cout << red_bg << "  " << reset_scr << std::endl;

    bool oddrow;
    for (int i1 = 0; i1 < 8; i1++) {
        for (int j = 0; j < 3; j++) {
            for (int i2 = 0; i2 < 4; i2++) {
                int i = i1*4 + i2;

                // Draw left border
                if (i2 == 0) {
                    std::cout << red_bg << "  ";
                    if (i1 < 4)
                        std::cout << white_bg << "  ";
                    else
                        std::cout << black_bg << "  ";
                }

                // Draw board
                oddrow = (((i - (i % 4))/4) % 2);
                if (!oddrow) {
                    std::cout << yellow_bg << "      ";
                }
                std::cout << red_bg;
                if (j == 0) {
                    if (i < 10) {
                        std::cout << i << "     ";
                    } else {
                        std::cout << i << "    ";
                    }
                }
                else if (j == 1) {
                    if ((*white_kings)[i]) {
                        std::cout << "  " << white_bg << white_text << "K " << red_bg << "  ";
                    }
                    else if ((*black_kings)[i]) {
                        std::cout << "  " << black_bg << black_text << "K " << red_bg << "  ";
                    }
                    else if ((*white_pieces)[i]) {
                        std::cout << "  " << white_bg << white_text << "  " << red_bg << "  ";
                    }
                    else if ((*black_pieces)[i]) {
                        std::cout << "  " << black_bg << black_text << "  " << red_bg << "  ";
                    }
                    else {
                        std::cout << "      ";
                    }
                }
                else if (j == 2) {
                    std::cout << "      ";
                }
                if (oddrow) {
                    std::cout << yellow_bg << "      ";
                }

                // Draw right border
                if (i2 == 3) {
                    if (i1 < 4)
                        std::cout << white_bg << "  ";
                    else
                        std::cout << black_bg << "  ";
                    std::cout << red_bg << "  ";
                }

                if (!((i+1) % 4))
                    std::cout << reset_scr << std::endl;
            }
        }
    }
    // Draw bottom border
    std::cout << red_bg << "  ";
    std::cout << black_bg;
    for (int i = 0; i < 52; i++)
        std::cout << " ";
    std::cout << red_bg << "  " << reset_scr << std::endl;
    std::cout << red_bg;
    for (int i = 0; i < 56; i++)
        std::cout << " ";
    std::cout << reset_scr << std::endl;
}

bool Board::no_piece(size_t i) {
    return !(*white_pieces)[i] && !(*black_pieces)[i];
}

bool Board::opponent_piece(Player p, size_t i) {
    return p == WHITE ? (*black_pieces)[i] : (*white_pieces)[i];
}

bool Board::is_white_piece(Player p, size_t i) {
    return (p == WHITE && (*white_pieces)[i]);
}

bool Board::is_black_piece(Player p, size_t i) {
    return (p == BLACK && (*black_pieces)[i]);
}

bool Board::is_white_king(Player p, size_t i) {
    return (p == WHITE && (*white_kings)[i]);
}

bool Board::is_black_king(Player p, size_t i) {
    return (p == BLACK && (*black_kings)[i]);
}

void Board::jumps_from_square(size_t i, Player p, std::vector<Move> &v) {
    bool oddrow = (((i - (i % 4))/4) % 2);
    size_t posInRow = i % 4;
    if ( i < 24 && (is_white_piece(p, i) || is_black_king(p, i)) ) {
        if (posInRow > 0 && oddrow && opponent_piece(p, i+3) && no_piece(i+7))
            v.push_back(Move(p, this, i, i+3, i+7));
        if (posInRow > 0 && !oddrow && opponent_piece(p, i+4) && no_piece(i+7))
            v.push_back(Move(p, this, i, i+4, i+7));
        if (posInRow < 3 && oddrow && opponent_piece(p, i+4) && no_piece(i+9))
            v.push_back(Move(p, this, i, i+4, i+9));
        if (posInRow < 3 && !oddrow && opponent_piece(p, i+5) && no_piece(i+9))
            v.push_back(Move(p, this, i, i+5, i+9));
    }
    if ( i > 7 && (is_black_piece(p, i) || is_white_king(p, i)) ) {
        if (posInRow > 0 && oddrow && opponent_piece(p, i-5) && no_piece(i-9))
            v.push_back(Move(p, this, i, i-5, i-9));
        if (posInRow > 0 && !oddrow && opponent_piece(p, i-4) && no_piece(i-9))
            v.push_back(Move(p, this, i, i-4, i-9));
        if (posInRow < 3 && oddrow && opponent_piece(p, i-4) && no_piece(i-7))
            v.push_back(Move(p, this, i, i-4, i-7));
        if (posInRow < 3 && !oddrow && opponent_piece(p, i-3) && no_piece(i-7))
            v.push_back(Move(p, this, i, i-3, i-7));
    }
    return;
}

void Board::shifts_from_square(size_t i, Player p, std::vector<Move> &v) {
    bool oddrow = (((i - (i % 4))/4) % 2);
    size_t posInRow = i % 4;
    if ( i < 28 && (is_white_piece(p, i) || is_black_king(p, i)) ) {
        if (posInRow > 0 && oddrow && no_piece(i+3))
            v.push_back(Move(p, this, i, i+3));
        if (!oddrow && no_piece(i+4))
            v.push_back(Move(p, this, i, i+4));
        if (oddrow && no_piece(i+4))
            v.push_back(Move(p, this, i, i+4));
        if (posInRow < 3 && !oddrow && no_piece(i+5))
            v.push_back(Move(p, this, i, i+5));
    }
    if ( i > 3 && (is_black_piece(p, i) || is_white_king(p, i)) ) {
        if (posInRow > 0 && oddrow && no_piece(i-5))
            v.push_back(Move(p, this, i, i-5));
        if (!oddrow && no_piece(i-4))
            v.push_back(Move(p, this, i, i-4));
        if (oddrow && no_piece(i-4))
            v.push_back(Move(p, this, i, i-4));
        if (posInRow < 3 && !oddrow && no_piece(i-3))
            v.push_back(Move(p, this, i, i-3));
    }
}

void Board::possible_moves(Player turn, std::vector<Move> &moves) {
    for (size_t i = 0; i < 32; i++)
        jumps_from_square(i, turn, moves);

    if (moves.size() == 0) {
        for (size_t i = 0; i < 32; i++)
            shifts_from_square(i, turn, moves);
    }
}

// Returns true if the move resulted in a crowned/kinged piece
bool Board::apply_move(Player turn, size_t start, size_t end) {
    bool ret = false;
    if (turn == WHITE) {
        (*white_pieces)[start] = 0;
        (*white_pieces)[end]   = 1;
        if ( end > 27 && !(*white_kings)[start] ) {
            ret = true;
            (*white_kings)[end] = 1;
        }
        if ((*white_kings)[start]) {
            (*white_kings)[start] = 0;
            (*white_kings)[end]   = 1;
        }
    } else {
        (*black_pieces)[start] = 0;
        (*black_pieces)[end]   = 1;
        if ( end < 4 && !(*black_kings)[start] ) {
            ret = true;
            (*black_kings)[end] = 1;
        }
        if ((*black_kings)[start]) {
            (*black_kings)[start] = 0;
            (*black_kings)[end]   = 1;
        }
    }
    return ret;
}

bool Board::apply_move(Player turn, size_t start, size_t jumped, size_t end) {
    if (turn == WHITE) {
        (*black_pieces)[jumped] = 0;
        (*black_kings )[jumped] = 0;
    } else {
        (*white_pieces)[jumped] = 0;
        (*white_kings )[jumped] = 0;
    }
    return apply_move(turn, start, end);
}

/* ------------ */
/* AI FUNCTIONS */
/* ------------ */
size_t Board::iterative_deepening(Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger) {
    size_t depth = 1;
    size_t finished = 0;
    Move m;
    bool maxDepthHit;
    while (!trigger) {
        maxDepthHit = alpha_beta_start(depth, maximize, moves, m, trigger);
        if (!trigger) {
            bestMove = m;
            finished = depth;
        }

        if (!maxDepthHit)
            trigger = true;

        depth += 2;
    }
    return finished;
}

bool Board::alpha_beta_start(size_t depth, Player maximize, std::vector<Move> &moves, Move &bestMove, bool &trigger) {
    int64_t alpha = std::numeric_limits<int64_t>::min();
    int64_t beta = std::numeric_limits<int64_t>::max();
    int64_t best = std::numeric_limits<int64_t>::min();
    bool maxDepthHit = false;
    for (auto move : moves) {
        int64_t v = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(maximize), trigger, maxDepthHit);
        if (v > best) {
            alpha = v;
            best = v;
            bestMove = move;
        }
        if (beta <= alpha)
            break;
    }
    return maxDepthHit;
}

int64_t Board::alpha_beta(size_t depth, int64_t alpha, int64_t beta, Player maximize, Player current, bool &trigger, bool &maxDepthHit) {
    if (trigger)
        return 0;

    if (depth == 0) {
        maxDepthHit = true;
        return score(maximize);
    }
    
    std::vector<Move> moves;
    possible_moves(current, moves);
    split_moves(moves, false);

    if (moves.size() == 0) {
        if (current == maximize)
            return std::numeric_limits<int64_t>::lowest();
        return std::numeric_limits<int64_t>::max();
    }

    if (current == maximize) {
        for (auto move : moves) {
            int64_t v = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(current), trigger, maxDepthHit);
            alpha = std::max(alpha, v);
            if (beta <= alpha)
                break;
        }
        return alpha;
    } else {
        for (auto move : moves) {
            int64_t v = move.board->alpha_beta(depth-1, alpha, beta, maximize, other_player(current), trigger, maxDepthHit);
            beta = std::min(beta, v) ;
            if (beta <= alpha)
                break;
        }
        return beta;
    }
}

int64_t Board::score(Player p) {
    int metric = 0;
    metric += score_0() * 1e12;
    metric += score_1() * 1e9;
    metric += score_2() * 1e6;
    metric += score_3() * 1e3;

    metric += rand() % 100;

    return p == WHITE ? metric : -metric;
}

int64_t Board::score_0() {
    const int king_weight = 2;
    const int base_weight = 3;
    return   king_weight * white_kings->count()
           - king_weight * black_kings->count()
           + base_weight * white_pieces->count()
           - base_weight * black_pieces->count();
}

int64_t Board::score_3() {
    int metric = 0;
    for (size_t i = 0; i < 32; i++) {
        int row = i/4;
        if ( (*white_pieces)[i] && !(*white_kings)[i] )
            metric += row*row;
        if ( (*black_pieces)[i] && !(*black_kings)[i] )
            metric -= (7-row)*(7-row);
        if ( row == 0 && (*white_pieces)[i] )
            metric += 5;
        if ( row == 7 && (*black_pieces)[i] )
            metric -= 5;
    }
    return metric;
}

int64_t Board::score_1() {
    return white_pieces->count() - black_pieces->count();
}

int64_t Board::score_2() {
    int64_t metric = 0;
    if ( white_pieces->count() < 4 || black_pieces->count() < 4) {
        if ( (white_pieces->count() < black_pieces->count()) ) {
            if ( (*white_pieces)[0] || (*white_pieces)[4] ) {
                metric += 9;
                metric -= (*black_pieces)[0]  * 3;
                metric -= (*black_pieces)[4]  * 3;
                metric -= (*black_pieces)[1]  * 1;
                metric -= (*black_pieces)[5]  * 1;
                metric -= (*black_pieces)[8]  * 1;
                metric -= (*black_pieces)[12] * 1;
            }
            if ( (*white_pieces)[27] || (*white_pieces)[31] ) {
                metric += 9;
                metric -= (*black_pieces)[27] * 3;
                metric -= (*black_pieces)[31] * 3;
                metric -= (*black_pieces)[19] * 1;
                metric -= (*black_pieces)[23] * 1;
                metric -= (*black_pieces)[26] * 1;
                metric -= (*black_pieces)[30] * 1;
            }
        }
        if ( (black_pieces->count() < white_pieces->count()) ) {
            if ( (*black_pieces)[0] || (*black_pieces)[4] ) {
                metric -= 9;
                metric += (*white_pieces)[0]  * 3;
                metric += (*white_pieces)[4]  * 3;
                metric += (*white_pieces)[1]  * 1;
                metric += (*white_pieces)[5]  * 1;
                metric += (*white_pieces)[8]  * 1;
                metric += (*white_pieces)[12] * 1;
            }
            if ( (*black_pieces)[27] || (*black_pieces)[31] ) {
                metric -= 9;
                metric += (*white_pieces)[27] * 3;
                metric += (*white_pieces)[31] * 3;
                metric += (*white_pieces)[19] * 1;
                metric += (*white_pieces)[23] * 1;
                metric += (*white_pieces)[26] * 1;
                metric += (*white_pieces)[30] * 1;
            }
        }
    }
    return metric;
}
