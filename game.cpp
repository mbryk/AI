#include "game.h"

Game::Game(char *state, int player_types[2]){
	board = new Board(state);
	srand(time(NULL));
	for(int i=0;i<2;i++){
		players[i] = new Player(player_types[i]);
		players[i]->color = i+1;
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
		Move *move = players[turn]->move(moves);
		over = board->makeMove(move);
		board->print();
		turn = 1-turn;
	}
	char *colors[2] = {"Red","Black"};
	std::cout<<"Game Over! "<<colors[over-1]<<" wins!"<<endl<<endl;

}
