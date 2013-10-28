#ifndef GAME_H
#define	GAME_H
#include "board.h"
#include "player.h"
#include "move.h"
#include <vector>
#include <string>
#include <iostream>
#include "time.h"
#include <cstdlib>

class Game{
public:
	Game(char*, int player_types[2], double t_limits[2], bool, bool);
	void play(int);
private:
	bool prints;
	Board *board;
	Player *players[2];
}; 

#endif	/* GAME_H */