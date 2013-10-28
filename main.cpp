#include "game.h"
#include "unistd.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	//string state = "a";
	char *state = NULL;
	int players[2] = {1,0};
	double t_limits[2] = {5,5};
	int c = 0;
	bool prints = true; bool debugPrint = false;
	while ((c = getopt (argc, argv, "l:s:p:od")) != -1){
		switch (c){
			case 's':
				//stateFile = openfile(optarg);
				//state = parse(stateFile);
				break;
			case 'p':
				players[0] = optarg[0] - '0';
				players[1] = optarg[1] - '0';
				break;
			case 'l':
				t_limits[0] = optarg[0] - '0';
				t_limits[1] = optarg[1] - '0';
			case 'o': //noprints = for testing purposes
				prints = false;
				players[0] = 0;
			case 'd':
				debugPrint = true;
		}
	}
	Game *game = new Game(state, players, t_limits, prints, debugPrint);
	char turnc;
	cout<<"Whose turn is it?"<<endl<<"1:Red, 2:Black"<<endl;
	cin>>turnc;
	int turn = turnc - '0' - 1;
	game->play(turn);	

	return 0;
}
