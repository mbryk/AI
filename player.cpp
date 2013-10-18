#include "player.h"
using namespace std;

Player::Player(){
}

Move *Player::move(vector<Move*> &moves){
	return (type)?getChoice(moves):chooseBest(moves);
}

Move *Player::chooseBest(vector<Move*> &moves){
	printMoves(moves);
	Move *move = moves.at(0);
	if(!move->nextJumps.empty()){
		move->nextJumpChosen = move->nextJumps.at(0);
	}
	return move;
}

Move *Player::getChoice(vector<Move*> &moves){
	printMoves(moves);
	char movec;
	int move;
	do{
		cin>>movec;
		move = movec - '0';


	}while(move>moves.size());
	return moves.at(move-1);
}

void Player::printMoves(vector<Move*> &moves){
	Move *move;
	int i = 1;
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		move = *it;
		cout<<i;
		move->print();
		i++;
	}
	cout<<endl;
}
