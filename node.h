#ifndef NODE_H
#define NODE_H

class Node{
public:
	double in;
	double activation;
	double delta;
	double bias;
	double** prev;
	double** next;
	Node(int);
	void assignNext(double*,int,int);
	void assignPrev(double*,int,int);
private:
	int index;
};

#endif