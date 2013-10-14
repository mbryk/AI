#ifndef MOVE_H
#define	MOVE_H
#include "square.h"

class Move{
public:
	Square origin;
	Square dest;
	Square jump;
	Move *nextJumps[];
	Move nextJumpChosen;
};

#endif	/* MOVE_H */
