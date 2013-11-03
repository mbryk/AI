#include "game.h"
using namespace std;
Game::Game(const char *state, int player_types[2], double t_limits[2], bool isprints, bool debugPrint, int hnum[2]){
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
	bool no_options;
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
		
		depth = 0;
		no_options = false;
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
					if(t_now.tv_sec-t_start.tv_sec > .7*players[turn]->t_lim) break;
					move = board->getBestMove(++depth, moves, no_options); //To all moves w/o nextJumps().
					if(no_options) break;
				}
			}
			catch (int t){
				depth--;
			}
			players[turn]->printMoves(moves);
			cout<<endl;
			move->print();
			cout<<" Chosen."<<endl<<"DEPTH:"<<depth<<" TOTAL TIME:";
			gettimeofday(&t_now, NULL);
			board->t_now = t_now;
			cout<<setprecision(6)<<board->tdiff()<<endl; 
		} else {
			move = players[turn]->getChoice(moves, true);
		}
		if(!board->makeMove(move)){ //FALSE = GAMEOVER
			fin = color;
			break;
		}
		board->deleteMoves(moves);
		if(prints) board->print();
		turn = 1-turn;
		
	}
	string colors[2] = {"Red","Black"};
	board->print();
	cout<<"Game Over! "<<colors[fin-1]<<" wins!"<<endl<<endl;

}
