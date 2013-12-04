#include <string>
#include <string.h>
#include "stdlib.h"
#include "unistd.h"
#include <iostream>
#include <fstream>

using namespace std;
int main(){
	string a = "bsetall";string b = "bset";
	ifstream in(a.c_str()); 
	ofstream ot(b.c_str());
	char *d[17];
	char *tmp;
	char *h = new char[500];
	char *c = new char[500];
	double tm;
	double dmax[17] = {3,9,42,1.75,95.5,100,6.28,7,4,1,1,1,1,1,1,1,1};
	ot.precision(3);
	ot<<fixed;
	int i,j;
	while(!in.eof()){
/*		strtok(c," ");
		for(int i=0;i<7;i++){
			d[i] = strtok(NULL," ");
		}
*/
		in.getline(c,500);
		strtok(c," ");

/*		for(int i=0;i<6;i++){
			in>>d[i];
		}
		ot<<d[0]<<" "
			<<d[1]<<" "
			<<d[2]<<" "
			<<d[3]<<" "
			<<d[5]<<" "
			<<d[4]<<endl;
*/
		for(i=0;i<16;i++){
			d[i] = strtok(NULL," ");
		}
		d[15][1] = '\0';
		in.getline(h,500);
		d[16] = strtok(h," ");
		d[16][1] = '\0';
		for (i=0;i<9;i++){
			tm = strtod(d[i],NULL);
			tm /= dmax[i];
			ot<<tm<<" ";
		}
		for(j=9;j<17;j++){
			ot<<d[j]<<" ";
		}
		ot<<endl;
	}
	ot.close();
	in.close();
	return 0;
}
	
	


