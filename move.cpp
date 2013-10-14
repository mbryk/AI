#include "move.h"

Move::Move(Square origin_s, Square dest_s, Square jumped_s){
	origin = origin_s;
	dest = dest_s;
	jumped = is_jmp;
	
}

Move::print(){
	string text = "] -> ["
	if(square.jumped) text = "] ^-> [";
	cout<<"["<<origin.x<<origin.y<<string<<dest.x<<dest.y<<"]";
	if(nextJumps != NULL){
		for(int i = 0; i<sizeof(nextJumps); i++)
			cout<<endl<<"\t"<<
				"["<<
				nextJumps[i]->origin.x << nextJumps[i]->origin.y <<
				"] ^-> ["<<
				nextJumps[i]->dest.x << nextJumps[i]->origin.y <<
				"]";
	}
	cout<<endl;

}