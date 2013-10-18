#include "game.h"

Game::Game(char *state, int player_types[2]){
	board = new Board(state);
	for(int i=0;i<2;i++){
		players[i] = new Player();
		players[i]->type = player_types[i];
		players[i]->color = i+1; //Red
	}
}

void Game::play(int turn){
	int over;
	board->print(); //Initial Print
	while(!over){
		vector<Move*> moves;
		board->getLegalMoves(players[turn]->color, moves);
		Move *move = players[turn]->move(moves);
		over = board->makeMove(move);
		board->print();
		turn = 1-turn;
	}
	char *colors[2] = {"Red","Black"};
	std::cout<<"Game Over! "<<colors[over-1]<<" wins!"<<endl;

}
