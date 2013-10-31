#include "game.h"

Game::Game(char *state, int player_types[2], double t_limits[2], bool isprints, bool debugPrint, int hnum[2]){
	board = new Board(state);
	board->debugPrint = debugPrint;
	srand(time(NULL));
	prints = isprints;
	for(int i=0;i<2;i++){
		players[i] = new Player(player_types[i]);
		players[i]->color = i+1;
		players[i]->t_lim = t_limits[i];
		players[i]->hnum = hnum[i];
		players[i]->prints = isprints; //testing

	}
}

void Game::play(int turn){
	int over, fin, depth, color;
	struct timeval t_start;
	time_t t_s;
	double t_diff;
	Move *move;
	board->print(); //Initial Print
	while(1){
		t_s = time(0);
		gettimeofday(&t_start, NULL);
		board->t_start = t_start;
		board->t_lim = players[turn]->t_lim;
		board->hnum = players[turn]->hnum;
		color = players[turn]->color;
		board->color = color;
		
		depth = 1;
		vector<Move*> moves;
		if(!board->getLegalMoves(color, moves)){ 
			fin = 3-color;
			break;
		}
		if(!players[turn]->type){ // For the computer
			try{
				do{
					move = board->getBestMove(depth++, moves); //To all moves w/o nextJumps().
					//t_diff = difftime(time(0), t_start);
					std::cout<<endl<<"d-"<<depth<<", t-"<<t_diff<<";   ";
					move->print();
				} while(1);
				//} while(depth<5);
			}
			catch (double t){
				cout<<endl<<"With "<<t<<" seconds left   "<<t_diff<<endl;
				move->print();
			}
			players[turn]->printMoves(moves);
		} else {
			move = players[turn]->getChoice(moves);
		}
		if(!board->makeMove(move)) //FALSE = GAMEOVER
			break;
		
		if(prints) board->print();
		turn = 1-turn;
		
		t_diff = difftime(time(0), t_s);
		std::cout<<"DEPTH:"<<depth<<" TOTAL TIME:"<<t_diff<<endl;
	}
	string colors[2] = {"Red","Black"};
	board->print();
	std::cout<<"Game Over! "<<colors[fin-1]<<" wins!"<<endl<<endl;

}
