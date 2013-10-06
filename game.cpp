#include "game.h"

Game::Game(string state, int turn, int player_types[]){
	board = new Board(state);
	players[0] = new Player(player_types[0]);
	players[1] = new Player(player_types[1]);
}

Game::play(turn){
	while(turn!=0){
		color = players[turn].color;
		legalMoves = board.getLegalMoves(color);
		move = players[turn].move(legalMoves);
		board.makeMove(move);
	}
}
