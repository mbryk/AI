#include "game.h"
using namespace std;

Game::Game(string state, int player_types[2]){
	board = new Board(state);
	for(int i=0;i<2;i++){
		players[i] = new Player();
		players[i].type = player_types[i];
		players[i].color = i+1; //Red
	}
}

Game::play(int turn){
	while(1){
		board.print();
		turn = 1-turn;
		Move *moves[];
		legalMoves = board.getLegalMoves(players[turn].color, moves);
		Move move = players[turn].move(legalMoves);
		board.makeMove(move);
	}
}
