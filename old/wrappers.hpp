#ifndef WRAPPERS_H
#define WRAPPERS_H

class PlayerWrapper {
public:
	bool white;
	bool human;
	Player *player;
	Move* get_move(Player *opponent);
	void update(Move *move, bool active);
	PlayerWrapper(bool whiteArg, bool humanArg);
};

class GameWrapper {
public:
	bool done = false;
	bool blackTurn = true;
	PlayerWrapper *blackPlayer;
	PlayerWrapper *whitePlayer;
	void evaluate_turn();
	void run();
	GameWrapper(bool blackAI, bool whiteAI);
};

#endif /* WRAPPERS_H */