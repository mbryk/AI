#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cmath>
#include "stdlib.h"
#include "unistd.h"
using namespace std;

int g(int in){
	int a = 1/(1+exp(in));
	return a;
}
int gprime(int in){
	int a = g(in);
	return (a*(1-a));
}
int s2i(char*){
	return 5;
}
void parseExample(char *c,int* x,int* y, int ins, int outs){
	x[0] = s2i(strtok(c," "));
	for(int i=1;i<ins;i++)
		x[i]=s2i(strtok(NULL," "));
	for(int j=0;j<outs;j++)
		y[j]=s2i(strtok(NULL," "));
}
void printNetwork(string file, int *L1, int *L2){

}

int main(int argc, char **argv){
	int layers = 3;
	int abias = -1;
	int i,j,k,l,alpha;	

	string netfile,exfile,outfile,astring;
	cout<<"What's yo network?"<<endl;
	cin>>netfile;
	cout<<"Where's yo examples?"<<endl;
	cin>>exfile;
	cout<<"Where should I send this baby?"<<endl;
	cin>>outfile;
	cout<<"So Who's Your Alpha Dog?"<<endl;
	cin>>astring;
	alpha = atoi(astring.c_str());

	int nodeAmts[3];
	ifstream net(netfile.c_str());
	char *str = new char[500];
	net.getline(str,500);
	nodeAmts[0] = s2i(strtok(str," "));
	for(i=1;i<layers;i++){
		nodeAmts[i] = s2i(strtok(NULL," "));
	}
	int inputs, hiddens, outputs;
	inputs = nodeAmts[0]; hiddens = nodeAmts[1]; outputs = nodeAmts[2];
	int *L1 = new int[inputs*hiddens]; // Weights 
	int *L2 = new int[hiddens*outputs]; //Weights

/*	for(j=0;j<hiddens;j++){ // Initialize Those Weights!
		for(i=0;i<inputs;i++){
			L1[i][j] = new Edge();
		}
		for(k=0;k<outputs;k++){
			L2[j][k] = new Edge();
		}
	}
*/
	int nmax = max(inputs,max(hiddens,outputs));
	Node *node[layers][nmax];

	for(l=0;l<layers;l++){ // Make those nodes!
		for(i=0;i<nodeAmts[l];i++){
			node[l][i] = new Node(i);
			switch(l){
				case 0: node[0][i]->assignNext(L1,hiddens); break;
				case 1: node[1][i]->assignPrev(L1,inputs); node[1][i]->assignNext(L2,outputs); break;
				case 2: node[2][i]->assignPrev(L2,hiddens); break;
			}
		}
	}

	/****** PARSE FILE FOR WEIGHTS **********/
	for(j=0;j<hiddens;j++){
		net.getline(str,500);
		node[1][j]->bias = s2i(strtok(str," "));
		for(i=0;i<inputs;i++){
			L1[i*j] = s2i(strtok(NULL," "));
		}
	}
	for(k=0;k<outputs;k++){
		net.getline(str,500);
		node[2][k]->bias = s2i(strtok(str," "));
		for(j=0;j<hiddens;j++){
			L2[j*k] = s2i(strtok(NULL," "));
		}
	}

	/******** EXAMPLES *****************/
	ifstream exstream(exfile.c_str());
	char *c = new char[500];
	int *x = new int[inputs];
	int *y = new int[outputs];
	int examples, epochs, e,delta;
	int *w;
	exstream.getline(c,500);
	epochs = s2i(strtok(c," "));
	examples = s2i(strtok(c," "));
	for(int t=0;t<epochs;t++){
		for(e=0;e<examples;e++){
			exstream.getline(c,500);
			parseExample(c,x,y,inputs,outputs);
			// Initialize Input Layer
			for(i=0;i<inputs;i++){ 
				node[0][i]->activation = x[i];
			}

			// Propagate To Output
			for(l=1;l<layers;l++){
				for(j=0;j<nodeAmts[l];j++){
					node[l][j]->in = 0;
					i=0;
					for (vector<int*>::iterator it = node[l][j]->prev.begin() ; it != node[l][j]->prev.end(); ++it){
						w = *it;
						node[l][j]->in += *w * node[l-1][i]->activation;
						i++;
					}
					node[l][j]->in += node[l][j]->bias;
					node[l][j]->activation = g(node[l][j]->in);
				}
			}

			// Compute Error At Output
			for(j=0;j<outputs;j++){
				node[2][j]->delta = gprime(node[2][j]->in) * y[j] - node[2][j]->activation;
			}

			// Propagate Deltas Back
			for(l=1;l>0;l--){
				for(i=0;i<nodeAmts[l];i++){
					delta = 0;
					j = 0;
					for (vector<int*>::iterator it = node[l][i]->next.begin() ; it != node[l][i]->next.end(); ++it){
						w = *it;
						delta += *w * node[l+1][j]->delta;
						j++;
					}
					node[l][i]->delta = gprime(node[2][j]->in)*delta;
				}
			}

			// Update Weights
			for(j=0;j<hiddens;j++){
				for(i=0;i<inputs;i++){
					L1[i*j] += alpha * node[0][i]->activation * node[1][j]->delta;
				} 
				node[1][j]->bias += alpha * abias * node[1][j]->delta;
			}

			for(k=0;k<outputs;k++){
				for(j=0;j<hiddens;j++){
					L2[j*k] += alpha * node[1][j]->activation * node[2][k]->delta;
				}
				node[2][k]->bias += alpha * abias * node[2][k]->delta;
			}
		}
		exstream.clear() ;
		exstream.seekg(0, ios::beg);
		exstream.getline(c,500);
	}	
	printNetwork(outfile,L1,L2);

	return 0;
}
