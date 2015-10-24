#include "game.h"
#include "board.h"
#include "constants.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	Game game;
	if (argc == 2) {
		game.load(argv[1]);
	}
	game.prompt();
	game.run();
}