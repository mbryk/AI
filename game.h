#ifndef GAME_H
#define	GAME_H
#include "board.h"
#include "player.h"
#include "move.h"
#include <vector>

class Game{
public:
	Game(char *state, int player_types[2]);
	void play(int);
private:
	Board *board;
	Player *players[];
}; 

#endif	/* GAME_H */