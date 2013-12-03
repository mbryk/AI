#include "node.h"

Node::Node(int i){
	in = 0; activation = 0; delta = 0; bias = 0; //Not actually that important, I think
	index = i;
}

void Node::assignPrev(double *weights, int sources, int cols){
	prev = new double*[sources];
	for(int i=0;i<sources;i++){
		prev[i] = &weights[i*cols+index];
	}
}

void Node::assignNext(double *weights, int targets, int cols){
	next = new double*[targets];
	for(int j=0;j<targets;j++){
		next[j] = &weights[index*cols+j];
	}
}