#include "move.h"

using namespace std;
Move::Move(Square *origin_s, Square *dest_s){
	origin = origin_s;
	dest = dest_s;	

	jumped = NULL; //INITIALIZED
	nextJumpChosen = NULL; //INITIALIZED
}

void Move::print(){
	Move *mv;
	string text = "] -> [";
	if(jumped != NULL) text = "] ^-> [";
	cout<<"["
		<<origin->x<<"."
		<<columnize(origin)
		<<text
		<<dest->x<<"."
		<<columnize(dest)
		<<"]";
	if(!nextJumps.empty()){
		for (vector<Move*>::iterator it = nextJumps.begin() ; it != nextJumps.end(); ++it){
			mv = *it;
			cout<<endl<<"\t"<<
				"["<<
				mv->origin->x << "." << columnize(mv->origin) <<
				"] ^-> ["<<
				mv->dest->x << "." << columnize(mv->dest) <<
				"]";
		}
	}
}

int Move::columnize(Square *sq){
	return sq->y*2 + 1 - (sq->x%2);
}