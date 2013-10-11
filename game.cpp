#include "game.h"
using namespace std;

Game::Game(string state, int player_types[2]){
	board = new Board(state);
	players[0] = new Player(player_types[0], 0);
	players[1] = new Player(player_types[1], 1);
}

Game::play(int turn){
	while(turn!=-1){
		color = players[turn].color;
		legalMoves = board.getLegalMoves(color);
		move = players[turn].move(legalMoves);
		board.makeMove(move);
	}
}
