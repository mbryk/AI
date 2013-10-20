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
	Game(char*, int player_types[2], bool);
	void play(int);
private:
	bool prints;
	Board *board;
	Player *players[];
	time_t time_start;
}; 

#endif	/* GAME_H */