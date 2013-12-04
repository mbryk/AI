#include <string>
#include <string.h>
#include "stdlib.h"
#include "unistd.h"
#include <iostream>
#include <fstream>

using namespace std;
int main(){
	string a = "mysetin";string b = "myset";
	ifstream in(a.c_str()); 
	ofstream ot;
	ot.open(b.c_str());
	string d[6];
	string h;
	char *c = new char[500];
	while(!in.eof()){
/*		strtok(c," ");
		for(int i=0;i<7;i++){
			d[i] = strtok(NULL," ");
		}
*/
		in>>h;
		for(int i=0;i<6;i++){
			in>>d[i];
		}
		ot<<d[0]<<" "
			<<d[1]<<" "
			<<d[2]<<" "
			<<d[3]<<" "
			<<d[5]<<" "
			<<d[4]<<endl;

	}
	ot.close();
	in.close();
	return 0;
}
	
	


