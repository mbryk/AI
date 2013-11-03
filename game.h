#ifndef GAME_H
#define	GAME_H
#include "board.h"
#include "player.h"
#include "move.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>  
#include "time.h"
#include "sys/time.h"
#include <cstdlib>

class Game{
public:
	Game(const char*, int player_types[2], double t_limits[2], bool, bool, int hnum[2]);
	void play(int);
private:
	bool prints;
	Board *board;
	Player *players[2];
	bool offerDraw(bool);
}; 

#endif	/* GAME_H */