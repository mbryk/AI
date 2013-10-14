#ifndef BOARD_H
#define	BOARD_H
#include "square.h"
#include "move.h"

class Board{

public:
	Board(char *state = NULL);
	void print();
	void getLegalMoves(int color, Move *moves[]);

private:
	Square *square[8][8];
	void getAdjacents(int row, int col, bool available, Square *spots[]);
	void getJumps(Square origin, Move *moves[]);
	void getNonJumps(Square origin, Move *moves[]);
	bool isValid(row, col);
	Square getNextSquare(Square origin,Square jumped);
	bool makeMove(Move move); // returns "Successful?"
	Board copy();
};

#endif	/* BOARD_H */
