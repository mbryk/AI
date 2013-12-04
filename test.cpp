// TO DO: Get rid of L1 and L2 in this file. 

#include "node.h"
#include <iostream>
#include <iomanip>
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

int main(int argc, char **argv){
	int layers = 3;
	double abias = -1;
	int i,j,k,l;	
	
	string netfile,exfile,outfile;

	cout<<"What's yo network?"<<endl;
	cin>>netfile;
	cout<<"Where's yo examples?"<<endl;
	cin>>exfile;
	cout<<"Where should I send this baby?"<<endl;
	cin>>outfile;

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
	double *L1 = new double[inputs*hiddens]; // Weights 
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

	/***** EXAMPLES *********/
	ifstream exstream(exfile.c_str());
	char *c = new char[500];
	double *x = new double[inputs];
	double *y = new double[outputs];
	int examples, epochs, e;
	double delta;
	double *w;
	exstream.getline(c,500);
	examples = atoi(strtok(c," "));
	bool score[examples][outputs];
	int output;
	double *A = new double[outputs]; double *B = new double[outputs]; double *C = new double[outputs]; double *D = new double[outputs];
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
			output = (node[2][j]->activation<.5)?0:1;
			if(y[j]==1){
				if(output==1) A[j]++;
				else C[j]++;
			} else{
				if(output==1) B[j]++;
				else D[j]++;
			}
		}
	}

	// PRINT RESULTS
	double acc,prec,recall,F1,Atot,Btot,Ctot,Dtot,acctot,prectot,recalltot,accmic,precmic,recmic,F1mic,F1mac;
	ofstream out(outfile.c_str());
	out<<fixed;
	for(j=0;j<outputs;j++){
		acc = (A[j]+D[j])/(A[j]+B[j]+C[j]+D[j]);
		prec = A[j]/(A[j]+B[j]);
		recall = A[j]/(A[j]+C[j]);
		F1 = (2*prec*recall)/(prec+recall);
		out<<setprecision(0)<<A[j]<<" "
			<<B[j]<<" "
			<<C[j]<<" "
			<<D[j]<<" ";

		out<<setprecision(3)<<acc<<" "
			<<prec<<" "
			<<recall<<" "
			<<F1<<endl;

		Atot += A[j]; Btot += B[j]; Ctot += C[j]; Dtot += D[j];
		acctot += acc; prectot += prec; recalltot += recall;
	}
	// Micro
	accmic = (Atot+Dtot)/(Atot+Btot+Ctot+Dtot);
	precmic = Atot/(Atot+Btot);
	recmic = Atot/(Atot+Ctot);
	F1mic = (2*precmic*recmic)/(precmic+recmic);
	out<<accmic<<" "<<precmic<<" "<<recmic<<" "<<F1mic<<endl;
	// Macro
	acctot /= outputs; prectot /= outputs; recalltot /= outputs;
	F1mac = (2*prectot*recalltot)/(prectot+recalltot);
	out<<acctot<<" "<<prectot<<" "<<recalltot<<" "<<F1mac<<endl;

	out.close();
}