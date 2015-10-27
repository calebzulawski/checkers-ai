#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum Player {WHITE, BLACK};

constexpr Player other_player(Player p) { return (p == WHITE) ? BLACK : WHITE; };

const std::string white_bg    = "\x1b[47m";
const std::string black_bg    = "\x1b[40m";
const std::string red_bg      = "\x1b[41m";
const std::string yellow_bg   = "\x1b[43m";
const std::string white_text  = "\x1b[30m";
const std::string black_text  = "\x1b[37m";
const std::string clear_scr   = "\033[2J\033[1H";
const std::string reset_scr   = "\x1b[0m";

#endif /* CONSTANTS_H */