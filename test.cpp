// TO DO: Get rid of L1 and L2 in this file. 

#include "node.h"
#include <vector>
#include <iostream>

int main(int argc, char **argv){
	int layers = 3;
	int abias = -1;
	int i,j,k,l;	
	
	string netfile,exfile,outfile;
	cout<<"What's yo network?"<<endl;
	cin>>netfile;
	cout<<"Where's yo examples?"<<endl;
	cin>>exfile;
	cout<<"Where should I send this baby?"<<endl;
	cin>>outfile;

	int nodeAmts[3] = {inputs,hiddens,outputs};
	ifstream net(netfile);
	char *str = new char[500];
	net.getline(str,500);
	nodeAmts[0] = s2i(strtok(str," "));
	for(i=1;i<layers;i++){
		nodeAmts[i] = s2i(strtok(NULL," "));
	}
	int inputs, hiddens, outputs;
	inputs = nodeAmts[0]; hiddens = nodeAmts[1]; outputs = nodeAmts[2];
	int *L1 = new int[inputs][hiddens]; // Weights 
	int *L2 = new int[hiddens][outputs]; //Weights

	/* for(j=0;j<hiddens;j++){ // Initialize Those Weights!
		for(i=0;i<inputs;i++){
			L1[i][j] = new Edge();
		}
		for(k=0;k<outputs;k++){
			L2[j][k] = new Edge();
		}
	}
	*/
	int max = max(inputs,hiddens,outputs);
	Node *node[layers][max];

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
			L1[i][j]->weight = s2i(strtok(NULL," "));
		}
	}
	for(k=0;k<outputs;k++){
		net.getline(str,500);
		node[2][k]->bias = s2i(strtok(str," "));
		for(j=0;j<hiddens;j++){
			L2[j][k]->weight = s2i(strtok(NULL," "));
		}
	}

	/***** EXAMPLES *********/
	ifstream exstream(exfile);
	char *c = new char[500];
	exstream.getline(c,500);
	examples = s2i(strtok(c," "));
	bool score[examples][outputs];
	for(e=0;e<examples;e++){
		extream.getline(c,500);
		parseExample(c,&x,&y);

		// Initialize Input Layer
		for(i=0;i<inputs;i++){ 
			node[0][i]->activation = e->x[i];
		}

		// Propagate To Output
		for(l=1;l<layers;l++){
			for(j=0;j<nodeAmts[l];j++){
				node[l][j]->in = 0;
				Edge *edge;
				i=0;
				for (vector<int*>::iterator it = node[l][j]->prev.begin() ; it != node[l][j]->prev.end(); ++it){
					node[l][j]->in += (*it) * node[l-1][i]->activation;
					i++;
				}
				node[l][j]->in += node[l][j]->bias;
				node[l][j]->activation = g(in);
			}
		}

		// Compute Error At Output
		for(j=0;j<outputs;j++){
			out = (node[2][j]->activation<.5)?0:1;
			score[e][j] = (y[j]==out);
		}
	}
	printStuff(outfile,score);	
}
int g(int in){
	int a = 1/(1+exp(in));
	return a;
}
int gprime(int in){
	int a = g(in);
	return (a*(1-a));
}
void parseExample(char *c,int** x,int** y, int ins, int outs){
	x[0] = s2i(strtok(c," "));
	for(int i=1;i<ins;i++)
		x[i]=s2i(strtok(NULL," "));
	for(int j=0;j<outs;j++)
		y[j]=s2i(strtok(NULL," "));
}

int s2i(char){
	return 5;
}

void printStuff(string file, int*){

}