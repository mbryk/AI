#ifndef BOARD_H
#define	BOARD_H
#include "square.h"
#include "move.h"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class Board{

public:
	Board(char*);
	void print();
	void getLegalMoves(int, vector<Move*>&);
	bool makeMove(Move*); // returns "Successful?"

private:
	Square *square[8][8];
	void getAdjacents(Square*, vector<Square*>&, bool);
	void getJumps(Square*, vector<Move*>&);
	void getNonJumps(Square*, vector<Move*>&);
	bool isValid(int, int);
	Square *getNextSquare(Square*,Square*);
	Board *copy();
};

#endif	/* BOARD_H */
