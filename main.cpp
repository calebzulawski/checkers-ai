#include "game.h"

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));
	//bool color = rand() % 2;
	
	Game game(true, false);
	game.run();
}