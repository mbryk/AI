#include "player.h"

Player::Player(player_type, player_color, time_limit){
	type = player_type;
	color = player_color;
}

Move Player::move(Move *moves[]){
	return (type = computer)?chooseBest(moves):getChoice(moves);
}

Player::chooseBest(Move *moves[]){
	pick random

}

Player::getChoice(Move *moves[]){
	printMoves(moves);
	cin<<move;
	return move;
}


