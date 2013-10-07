#ifndef MOVE_H
#define	MOVE_H
#include "square.h"

class Move{
public:
	Square origin;
	Square destination;
	Move nextJump[];
}

#endif	/* MOVE_H */
