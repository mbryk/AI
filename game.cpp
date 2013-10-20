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
	time_start = time(0);
}

void Game::play(int turn){
	int over, color, time_diff, depth;
	Move *move;
	board->print(); //Initial Print
	while(1){
		depth = 1;
		color = players[turn]->color;
		vector<Move*> moves;
		board->getLegalMoves(color, moves);
		if(!players[turn]->type){ // For the computer
			//while(time_diff<(.5*players[turn]->time_limit)){
			//while(time_diff<(.5*5000)){
			while(depth<5){
				move = board->getBestMove(color, depth++, moves); //To all moves w/o nextJumps().
				time_diff = time(0)-time_start;
			}
		} else {
			move = players[turn]->getChoice(moves);
		}
		if(!board->makeMove(move)) //FALSE = GAMEOVER
			break;
		
		if(prints) board->print();
		turn = 1-turn;
	}
	string colors[2] = {"Red","Black"};
	if(!prints) board->print();
	std::cout<<"Game Over! "<<colors[color-1]<<" wins!"<<endl<<endl;

}
