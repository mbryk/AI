#include "game.h"

Game::Game(char *state, int player_types[2], int t_limits[2], bool isprints, bool debugPrint, int hnum[2]){
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
	bool no_options = false;
	struct timeval t_start, t_now;
	Move *move;
	board->print(); //Initial Print
	while(1){
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
				//while(depth<5){
				while(1){
					gettimeofday(&t_now, NULL);
					if(t_now.tv_sec-t_start.tv_sec > .6*players[turn]->t_lim) break;
					move = board->getBestMove(depth++, moves, no_options); //To all moves w/o nextJumps().
					if(no_options) break;
				}
			}
			catch (int t){
				move->print();
				cout<<" Chosen"<<endl;
			}
			players[turn]->printMoves(moves);
		} else {
			move = players[turn]->getChoice(moves, true);
		}
		if(!board->makeMove(move)){ //FALSE = GAMEOVER
			fin = color;
			break;
		}
		
		if(prints) board->print();
		turn = 1-turn;
		
		std::cout<<"DEPTH:"<<depth<<" TOTAL TIME:";
		gettimeofday(&t_now, NULL);
		cout<<t_now.tv_sec-t_start.tv_sec<<"  "<<t_now.tv_usec-t_start.tv_usec<<endl; 
	}
	string colors[2] = {"Red","Black"};
	board->print();
	std::cout<<"Game Over! "<<colors[fin-1]<<" wins!"<<endl<<endl;

}
