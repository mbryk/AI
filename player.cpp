#include "player.h"
using namespace std;

Player::Player(int ptype){
	type = ptype;
}

Move *Player::move(vector<Move*> &moves){
	return (type)?getChoice(moves):chooseBest(moves);
}

Move *Player::chooseBest(vector<Move*> &moves){
	printMoves(moves);
	int num = rand() % moves.size();
	Move *move = moves.at(num);
	if(!move->nextJumps.empty()){
		num = rand() % move->nextJumps.size();
		move->nextJumpChosen = move->nextJumps.at(num);
	}
	return move;
}

Move *Player::getChoice(vector<Move*> &moves){
	printMoves(moves);
	string movec;
	int movenum;
	do{
		cin>>movec;
		movenum = atoi(movec.c_str());
	}while(movenum>moves.size() || movenum<1);

	Move *move = moves.at(movenum-1);
	if(!moves.at(movenum-1)->nextJumps.empty()){
		if(move->nextJumps.size()>1){
			cout<<endl;
			move->nextJumpChosen = getChoice(move->nextJumps);
		} else move->nextJumpChosen = move->nextJumps.at(0);
	}
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
