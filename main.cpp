#include "game.h"
int main(int argc, char *argv[]){
	string state = "";
	while ((c = getopt (argc, argv, "s:")) != -1){
		switch (c){
			case 's':
				stateFile = openfile(optarg);
				state = parse(stateFile);
				break;
		}
	}
	Game game(state);	
	game.play();	

	return 0;
}
