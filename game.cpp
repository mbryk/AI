#include "game.h"

Game::Game(char *state, int player_types[2], bool isprints){
	board = new Board(state);
	srand(time(NULL));
	prints = isprints;
	for(int i=0;i<2;i++){
		players[i] = new Player(player_types[i]);
		players[i]->color = i+1;
		players[i]->prints = isprints; //testing
	}
}

void Game::play(int turn){
	int over;
	board->print(); //Initial Print
	while(!over){
		vector<Move*> moves;
		board->getLegalMoves(players[turn]->color, moves);
		if(moves.empty()){
			over = 2-turn;
			break;
		}
		if(!players[turn]->type) 
			board->assignVals(moves, players[turn]->color); //To all moves w/o nextJumps().
		Move *move = players[turn]->move(moves);
		over = board->makeMove(move);
		if(prints) board->print();
		turn = 1-turn;
	}
	string colors[2] = {"Red","Black"};
	if(!prints) board->print();
	std::cout<<"Game Over! "<<colors[over-1]<<" wins!"<<endl<<endl;

}
