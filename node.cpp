#include "node.h"

Node::Node(int i){
	index = i;
}

void Node::assignPrev(int *weights, int sources){
	for(int i=0;i<sources;i++){
		prev.push_back(&weights[i*index]);
	}
}

void Node::assignNext(int *weights, int targets){
	for(int j=0;j<targets;j++){
		next.push_back(&weights[index*j]);
	}
}