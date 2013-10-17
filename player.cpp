#include "player.h"
using namespace std;

Player::Player(){
}

Move *Player::move(vector<Move*> &moves){
	return (type)?getChoice(moves):chooseBest(moves);
}

Move *Player::chooseBest(vector<Move*> &moves){
	return moves.at(0);
}

Move *Player::getChoice(vector<Move*> &moves){
	printMoves(moves);
	int movenum;
	cin>>movenum;
	return moves.at(movenum);
}

void Player::printMoves(vector<Move*> &moves){
	for(int i=0;i<sizeof(moves); i++){
		moves[i]->print();
	}
}
