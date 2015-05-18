#include "board.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	auto ai = new Player(true);
	auto opp = new Player(false);
	display_board(ai,opp);
}