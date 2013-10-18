#include "game.h"
#include "unistd.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	//string state = "a";
	char *state = NULL;
	int players[2] = {1,0};
	int c = 0;
	bool prints = true;
	while ((c = getopt (argc, argv, "s:p:o")) != -1){
		switch (c){
			case 's':
				//stateFile = openfile(optarg);
				//state = parse(stateFile);
				break;
			case 'p':
				players[0] = optarg[0] - '0';
				players[1] = optarg[1] - '0';
				break;
			case 'o': //noprints = for testing purposes
				prints = false;
				players[0] = 0;
		}
	}
	Game *game = new Game(state, players, prints);
	char turnc;
	cout<<"Whose turn is it?"<<endl<<"1:Red, 2:Black"<<endl;
	cin>>turnc;
	int turn = turnc - '0' - 1;
	game->play(turn);	

	return 0;
}
