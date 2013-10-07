#include "game.h"
int main(int argc, char *argv[]){
	string state = "";
	char players[] = ["1","0"];
	while ((c = getopt (argc, argv, "s:p:")) != -1){
		switch (c){
			case 's':
				stateFile = openfile(optarg);
				state = parse(stateFile);
				break;
			case 'p':
				players[0] = optarg[0];
				players[1] = optarg[1];
				break;
		}
	}
	Game game(state, players);
	int turn;	
	cout<<"Whose turn is it?"<<endl<<"0:Red, 1:Black"<<endl;
	cin>>turn;
	game.play(turn);	

	return 0;
}
