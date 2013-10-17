#ifndef MOVE_H
#define	MOVE_H
#include "square.h"
#include <vector>
#include <cstddef>
#include <string>
#include <iostream>

class Move{
public:
	Square *origin, *dest, *jumped;
	std::vector<Move*> nextJumps;
	Move *nextJumpChosen;
	Move(Square*, Square*);
	void print();
};

#endif	/* MOVE_H */
