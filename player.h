#ifndef PLAYER_H
#define	PLAYER_H
#include "move.h"
#include <vector>
#include <iostream>

class Player{
public:
	bool type; //0=Comp, 1=Human
	int color;
	int timelim;

	Player();
	Move *move(std::vector<Move*>&);

private:
	Move *chooseBest(std::vector<Move*>&);
	Move *getChoice(std::vector<Move*>&);
	void printMoves(std::vector<Move*>&);
};

#endif	/* PLAYER_H */