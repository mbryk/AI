#include "player.h"

Player::Player(int time_limit = 0){
}

Player::move(Move *moves[]){
	return (type = computer)?chooseBest(moves):getChoice(moves);
}

Player::chooseBest(Move *moves[]){
	pick random(moves);

}

Player::getChoice(Move *moves[]){
	printMoves(moves);
	cin>>move;
	return move;
}

Player::printMoves(Move *moves[]){
	for(i=0;i<sizeof(moves); i++){
		moves[i].print();
	}
}
