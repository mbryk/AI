#include "game.h"
#include "unistd.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	//string state = "a";
	char *state = NULL;
	int players[2] = {1,0};
	int t_limits[2] = {5,5};
	int hnum[2] = {3,3};
	int c = 0;
	bool prints = true; bool debugPrint = false;
	while ((c = getopt (argc, argv, "t:l:s:p:h:od")) != -1){
		switch (c){
			case 's':
				//stateFile = openfile(optarg);
				//state = parse(stateFile);
				break;
			case 'p':
				players[0] = optarg[0] - '0';
				players[1] = optarg[1] - '0';
				break;
			case 't':
				t_limits[0] = atoi(optarg);
				break;
			case 'l':
				t_limits[1] = atoi(optarg);
				break;
			case 'o': //noprints = for testing purposes
				prints = false;
				players[0] = 0;
				break;
			case 'd':
				debugPrint = true;
				break;
			case 'h':
				hnum[0] = optarg[0] - '0';
				hnum[1] = optarg[1] - '0';
				break;
		}
	}
	Game *game = new Game(state, players, t_limits, prints, debugPrint, hnum);
	char turnc;
	int turn;
	cout<<"Whose turn is it?"<<endl<<"1:Red, 2:Black"<<endl;
	do{
		cin>>turnc;
		turn = turnc - '0' - 1;
	} while (turn!=0 && turn!=1);

	game->play(turn);	

	return 0;
}
