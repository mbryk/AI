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
	bool no_options, draw;
	struct timeval t_start, t_now;
	Move *move, *tempMove;
	board->print(); //Initial Print
	vector<Move*> moves;
	while(1){
		gettimeofday(&t_start, NULL);
		board->t_start = t_start;
		board->t_lim = players[turn]->t_lim;
		board->hnum = players[turn]->hnum;
		color = players[turn]->color;
		board->color = color;
		depth = 0;
		no_options = false;
		
		if(!board->getLegalMoves(color, moves)){ 
			fin = 3-color;
			break;
		}
		if(!players[turn]->type){ // For the computer
				//while(depth<8){
			while(1){
				if(board->draw){
					if(offerDraw(players[1-turn]->type)){
						cout<<"Congrats! It's a draw!"<<endl; //offer it to the other person
						exit(0);
					} else {
						cout<<"Denied!"<<endl;
					}
				}
				gettimeofday(&t_now, NULL);
				if(t_now.tv_sec-t_start.tv_sec > .7*players[turn]->t_lim) break;
				if((tempMove = board->getBestMove(++depth, moves, no_options))!=NULL){
					move = tempMove;
					if(no_options) break;
				} else {
					depth--;
				}
			}
			players[turn]->printMoves(moves);
			cout<<endl;

			move->print();
			cout<<" Chosen."<<endl<<"DEPTH:"<<depth<<" TOTAL TIME:";
			gettimeofday(&t_now, NULL);
			board->t_now = t_now;
			cout<<setprecision(6)<<board->tdiff()<<endl; 
		} else {
			do{
				move=players[turn]->getChoice(moves, true);
				if(move==NULL) {
					if(offerDraw(players[1-turn]->type)){
						cout<<"Congrats! It's a Draw!"<<endl;
						exit(-1);
					} else { cout<<"Denied!"<<endl;}
				}
			}while(move==NULL);
		}
		board->draw = false;
		if(!board->makeMove(move, true)){ //FALSE = GAMEOVER
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

bool Game::offerDraw(bool ptype){
	if(!ptype) return board->draw;
	char drawc; int draw;
	cout<<"Would you like to accept a draw? (0 for no, 1 for yes)"<<endl;
	do{
		cin>>drawc;
		draw = drawc - '0';
	} while (draw!=0 && draw!=1);

	return (bool) draw;
}