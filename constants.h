#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum Player {WHITE, BLACK};

const std::string white_bg = "\x1b[47m";
const std::string black_bg = "\x1b[40m";
const std::string white_text  = "\x1b[30m";
const std::string black_text  = "\x1b[37m";
const std::string clear_scr   = "\033c";

#endif /* CONSTANTS_H */