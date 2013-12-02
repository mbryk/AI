#ifndef NODE_H
#define NODE_H

#include "edge.h"
#include <vector>

class Node{
public:
	int activation;
	int delta;
	vector<Edge*> prev;
	vector<Edge*> next;
	Node(int);
	assignNext(Edge**);
	assignPrev(Edge**);
private:
	int index;
}
#endif NODE_H