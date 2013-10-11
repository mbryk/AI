#ifndef GAME_H
#define	GAME_H
#include "board.h"
#include "player.h"

class Game{
public:
	Game(std::string state, int player_types[2]);
	void play(int turn);
private:
	Board *board;
	Player *players[];
}; 

#endif	/* GAME_H */