#include "board.hpp"
#include "ai.hpp"
#include "wrappers.hpp"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));
	bool color = rand() % 2;
	
	auto game = new GameWrapper(color, !color);
	game->run();
}