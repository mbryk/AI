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
	bool getLegalMoves(int, vector<Move*>&);
	bool makeMove(Move*); // returns "Game Over"
	Move *getBestMove(int, int, vector<Move*>&);

private:
	Square *square[8][4];
	void printRow(int);
	void getAdjacents(Square*, vector<Square*>&, bool);
	void getJumps(Square*, vector<Move*>&);
	void getNonJumps(Square*, vector<Move*>&);
	bool isValid(int, int);
	Square *getNextSquare(Square*,Square*);
	void emptySquare(Square*);
	bool checkKing(Square*);
	bool terminalTest(int);
	Board *copy();
	int miniMaxVal(Move*, int, bool, int);
	int evaluateBoard(int);
	int countPieces(int);

};

#endif	/* BOARD_H */
