#include "player.h"
using namespace std;

Player::Player(int ptype){
	type = ptype;
}

Move *Player::getChoice(vector<Move*> &moves, bool inputReq){ //For Dumb Player
	int movenum;
	if(moves.size()>1 || (moves.size()==1 && inputReq)) {
		printMoves(moves);
		string movec;
		do{
			cin>>movec;
			movenum = atoi(movec.c_str());
		} while(movenum>moves.size() || movenum<1);	
	} else movenum = 1;
	
	Move *move = moves.at(movenum-1);
	if(!move->nextJumps.empty())
		move->nextJumpChosen = getChoice(move->nextJumps, false);
	return move;
}

void Player::printMoves(vector<Move*> &moves){
	Move *move;
	int i = 1;
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		move = *it;
		cout<<i<<": ";
		move->print();
		cout<<endl;
		i++;
	}
	cout<<endl;
}
