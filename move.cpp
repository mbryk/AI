#include "move.h"

using namespace std;
Move::Move(Square *origin_s, Square *dest_s){
	origin = origin_s;
	dest = dest_s;	
	jumped = NULL;
}

void Move::print(){
	Move *mv;
	string text = "] -> [";
	if(jumped != NULL) text = "] ^-> [";
	cout<<"["
		<<origin->x
		<<origin->y
		<<text
		<<dest->x
		<<dest->y
		<<"]";
	if(!nextJumps.empty()){
		for (vector<Move*>::iterator it = nextJumps.begin() ; it != nextJumps.end(); ++it){
			mv = *it;
			cout<<endl<<"\t"<<
				"["<<
				mv->origin->x << mv->origin->y <<
				"] ^-> ["<<
				mv->dest->x << mv->origin->y <<
				"]";
		}
	}
	cout<<endl;

}