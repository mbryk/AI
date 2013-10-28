#include "game.h"

Game::Game(char *state, int player_types[2], double t_limits[2], bool isprints, bool debugPrint){
	board = new Board(state);
	board->debugPrint = debugPrint;
	srand(time(NULL));
	prints = isprints;
	for(int i=0;i<2;i++){
		players[i] = new Player(player_types[i]);
		players[i]->color = i+1;
		players[i]->t_lim = t_limits[i];
		players[i]->prints = isprints; //testing

	}
}

void Game::play(int turn){
	int over, color, depth;
	time_t t_start;
	double t_diff;
	Move *move;
	board->print(); //Initial Print
	while(1){
		t_start = time(0);
		board->t_start = t_start;
		
		depth = 1;
		color = players[turn]->color;
		vector<Move*> moves;
		if(!board->getLegalMoves(color, moves)){ 
			color = 3-color;
			break;
		}
		if(!players[turn]->type){ // For the computer
			do{
				move = board->getBestMove(color, depth++, moves); //To all moves w/o nextJumps().
				t_diff = difftime(time(0), t_start);
				//std::cout<<"d-"<<depth<<"t-"<<t_diff<<";";
			} while(t_diff<(.5*players[turn]->t_lim));
			//} while(depth<4);
			players[turn]->printMoves(moves);
		} else {
			move = players[turn]->getChoice(moves);
		}
		if(!board->makeMove(move)) //FALSE = GAMEOVER
			break;
		
		if(prints) board->print();
		turn = 1-turn;
		
		t_diff = difftime(time(0), t_start);
		std::cout<<"DEPTH:"<<depth<<" TOTAL TIME:"<<t_diff<<endl;
	}
	string colors[2] = {"Red","Black"};
	board->print();
	std::cout<<"Game Over! "<<colors[color-1]<<" wins!"<<endl<<endl;

}
