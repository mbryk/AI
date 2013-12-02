#ifndef NODE_H
#define NODE_H

#include <vector>

class Node{
public:
	int in;
	int activation;
	int delta;
	int bias;
	std::vector<int*> prev;
	std::vector<int*> next;
	Node(int);
	void assignNext(int*,int);
	void assignPrev(int*,int);
private:
	int index;
};

#endif