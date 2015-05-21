#ifndef WRAPPERS_H
#define WRAPPERS_H

class PlayerWrapper {
public:
	bool white;
	bool human;
	Player *player;
	Move* get_move(Player *opponent);
	PlayerWrapper(bool whiteArg, bool humanArg);
};

#endif /* WRAPPERS_H */