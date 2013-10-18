#ifndef PLAYER_H
#define	PLAYER_H
#include "move.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>

class Player{
public:
	bool type; //0=Comp, 1=Human
	int color;
	int timelim;
	bool prints; //testing

	Player(int);
	Move *move(std::vector<Move*>&);

private:
	Move *chooseBest(std::vector<Move*>&);
	int evaluateMove(Move*);
	Move *getChoice(std::vector<Move*>&);
	void printMoves(std::vector<Move*>&);
};

#endif	/* PLAYER_H */