#include <string>
#include <string.h>
#include "stdlib.h"
#include "unistd.h"
#include <iostream>
#include <fstream>

using namespace std;
int main(){
	string a = "strikeset";string b = "bsetall";
	ifstream in(a.c_str()); 
	ofstream ot(b.c_str());
	char *d[5];
	char *tmp;
	char *h = new char[500];
	char *c = new char[500];
	double ii[5];
	double tm;
	//double *dmax = new double[5];
	double dmax[5] = {7000,17,27.5,78.7,1};
	ot.precision(3);
	ot<<fixed;
	int i,j;
	double total,k;
	total=0;
	char st;
	int s = 0;
	while(!in.eof()){
		k++;
/*		strtok(c," ");
		for(int i=0;i<7;i++){
			d[i] = strtok(NULL," ");
		}
*/
		in.getline(c,500);
		strtok(c," "); strtok(NULL," ");

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
		for(i=0;i<5;i++){
			d[i] = strtok(NULL," ");
			tm = strtod(d[i],NULL);
			if(i==0){
				st = (tm>150)?'1':'0';
			} else
				ii[i] = tm/dmax[i];
			//dmax[i] = max(dmax[i],tm);
		}
		ot<<ii[1]<<" "
			<<ii[2]<<" "
			<<ii[3]<<" "
			<<ii[4]<<" "
			<<st<<endl;		
//		ot<<endl;
	}
	//cout<<(total/k)<<endl;
	ot.close();
	in.close();
	return 0;
}
	
	


