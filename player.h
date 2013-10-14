#ifndef PLAYER_H
#define	PLAYER_H
#include "move.h"

class Player{
public:
	int type; //0=Comp, 1=Human
	int color;
	int timelim;

	Player(int time_limit = 0);
	Move move(Move *moves[]);

private:
	Move chooseBest(Move *moves[]);
	Move getChoice(Move *moves[]);
	void printMoves(Move *moves[]);
};

#endif	/* PLAYER_H */