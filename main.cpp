#include "game.h"

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));
	//bool color = rand() % 2;
	
	auto game = new GameWrapper(true, false);
	game->run();
}