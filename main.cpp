#include "game.h"
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const char *parseFile(char *fileName)
{
	ifstream in(fileName);
	if(!in.good()){
		cerr<<"Bad Board File"<<endl; exit(-1);
	}
	string contents((std::istreambuf_iterator<char>(in)), 
    istreambuf_iterator<char>());
	return contents.c_str();
}

int main(int argc, char *argv[]){
	const char *state = (char*) "1111.1111.1111.----.----.2222.2222.2222";
	int players[2] = {1,0};
	double t_limit = 5;
	int hnum[2] = {2,2};
	int c = 0;
	while ((c = getopt (argc, argv, "b:p:h:")) != -1){
		switch (c){
			case 'b':
				state = parseFile(optarg);
				break;
			case 'p':
				players[0] = optarg[0] - '0';
				players[1] = optarg[1] - '0';
				break;
			case 'h':
				hnum[0] = optarg[0] - '0';
				hnum[1] = optarg[1] - '0';
				break;
		}
	}
	
	char turnc; string t_limit_string;
	int turn;
	int t_limit_int;
	cout<<"Whose turn is it?"<<endl<<"1:Red, 2:Black"<<endl;
	do{
		cin>>turnc;
		turn = turnc - '0' - 1;
	} while (turn!=0 && turn!=1);

	cout<<"What is the program's time limit? (in seconds)"<<endl;
	do{
		cin>>t_limit_string;
		t_limit_int = atoi(t_limit_string.c_str());
	} while(t_limit_int<=0);
	t_limit = (double) t_limit_int;

	Game *game = new Game(state, players, hnum, t_limit);
	game->play(turn);	

	return 0;
}