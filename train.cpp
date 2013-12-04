#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cmath>
#include "stdlib.h"
#include "unistd.h"
using namespace std;

double g(double in){
	double a = 1/(1+exp(-in));
	return a;
}
double gprime(double in){
	double a = g(in);
	return (a*(1-a));
}
void parseExample(char *c,double* x,double* y, int ins, int outs){
	x[0] = strtod(strtok(c," "),NULL);
	for(int i=1;i<ins;i++)
		x[i]=strtod(strtok(NULL," "),NULL);
	for(int j=0;j<outs;j++)
		y[j]=strtod(strtok(NULL," "),NULL);
}
void printNetwork(string file, double *L1, double *L2){
}

int main(int argc, char **argv){
	int layers = 3;
	double abias = -1;
	int i,j,k,l,epochs;
	double alpha;	

	string netfile,exfile,outfile,astring,epstring;
	cout<<"Network:\t";	cin>>netfile;
	cout<<"Training Set:\t"; cin>>exfile;
	cout<<"Output File:\t";	cin>>outfile;
	cout<<"Alpha:\t\t";	cin>>astring;
	cout<<"Epochs:\t\t"; cin>>epstring;
	// TEMP
//	netfile = "net1init"; exfile = "ex1train"; outfile = "net1out"; astring = ".1";
	alpha = strtod(astring.c_str(),NULL);
	epochs = atoi(epstring.c_str());

	int nodeAmts[3];
	ifstream net(netfile.c_str());
	char *str = new char[500];
	net.getline(str,500);
	nodeAmts[0] = atoi(strtok(str," "));
	for(i=1;i<layers;i++){
		nodeAmts[i] = atoi(strtok(NULL," "));
	}
	int inputs, hiddens, outputs;
	inputs = nodeAmts[0]; hiddens = nodeAmts[1]; outputs = nodeAmts[2];
	double *L1 = new double[inputs*hiddens]; //Weights 
	double *L2 = new double[hiddens*outputs]; //Weights
	int nmax = max(inputs,max(hiddens,outputs));
	Node *node[layers][nmax];

	for(l=0;l<layers;l++){ // Make those nodes!
		for(i=0;i<nodeAmts[l];i++){
			node[l][i] = new Node(i);
			switch(l){
				case 0: node[0][i]->assignNext(L1,hiddens); break;
				case 1: node[1][i]->assignPrev(L1,inputs,hiddens); node[1][i]->assignNext(L2,outputs); break;
				case 2: node[2][i]->assignPrev(L2,hiddens,outputs); break;
			}
		}
	}

	/****** PARSE FILE FOR WEIGHTS **********/
	for(j=0;j<hiddens;j++){
		net.getline(str,500);
		node[1][j]->bias = strtod(strtok(str," "),NULL);
		for(i=0;i<inputs;i++){
			L1[i*hiddens+j] = strtod(strtok(NULL," "),NULL);
		}
	}
	for(k=0;k<outputs;k++){
		net.getline(str,500);
		node[2][k]->bias = strtod(strtok(str," "),NULL);
		for(j=0;j<hiddens;j++){
			L2[j*outputs+k] = strtod(strtok(NULL," "),NULL);
		}
	}

	/******** EXAMPLES *****************/
	ifstream exstream(exfile.c_str());
	char *c = new char[500];
	double *x = new double[inputs];
	double *y = new double[outputs];
	int examples, e;
	double delta;
	double *w;
	exstream.getline(c,500);
	examples = atoi(strtok(c," "));
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
					for(i=0;i<nodeAmts[l-1];i++){
						w = node[l][j]->prev[i];
						node[l][j]->in += (*w) * node[l-1][i]->activation;
					}
					node[l][j]->in += (node[l][j]->bias * abias);
					node[l][j]->activation = g(node[l][j]->in);
				}
			}

			// Compute Error At Output
			for(j=0;j<outputs;j++){
				node[2][j]->delta = gprime(node[2][j]->in) * (y[j] - node[2][j]->activation);
			}

			// Propagate Deltas Back
			for(l=1;l>0;l--){
				for(i=0;i<nodeAmts[l];i++){
					delta = 0;
					for(j=0;j<nodeAmts[l+1];j++){
						w = node[l][i]->next[j];
						delta += (*w) * node[l+1][j]->delta;
					}
					node[l][i]->delta = gprime(node[l][i]->in) * delta;
				}
			}

			// Update Weights
			for(j=0;j<hiddens;j++){
				for(i=0;i<inputs;i++){
					L1[i*hiddens+j] += alpha * node[0][i]->activation * node[1][j]->delta;
				} 
				node[1][j]->bias += alpha * abias * node[1][j]->delta;
			}

			for(k=0;k<outputs;k++){
				for(j=0;j<hiddens;j++){
					L2[j*outputs+k] += alpha * node[1][j]->activation * node[2][k]->delta;
				}
				node[2][k]->bias += alpha * abias * node[2][k]->delta;
			}
		}
		exstream.clear() ;
		exstream.seekg(0, ios::beg);
		exstream.getline(c,500);
	}	

	printNetwork(outfile,L1,L2);

	ofstream out(outfile.c_str());
	// Print network for debugging
	out<<inputs<<" "<<hiddens<<" "<<outputs<<endl;
	out.precision(3);
	out<<fixed;
	Node *n;
	for(l=1;l<layers;l++){
		for(j=0;j<nodeAmts[l];j++){
			n = node[l][j];
			out<<n->bias<<" ";
			for(i=0;i<nodeAmts[l-1];i++){
				out<<*(n->prev[i])<<" ";
			}
			out<<endl;
		}
	}
	out.close();

	return 0;
}
