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
	bool debugPrint;
	time_t t_start;
	void print();
	bool getLegalMoves(int, vector<Move*>&);
	bool makeMove(Move*); // returns "Game Over"
	Move *getBestMove(int, int, vector<Move*>&, int);

private:
	Square *square[8][4];
	bool end_section;
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
	int miniMaxVal(Move*, int, bool, int, int, int, int);
	int evaluateBoard(int, int);
	int h1(int);
	int h2(int);
	int h3(int);
	int h3begin(int);
	int h3end(int);
	int distances(int,int,int);
	int countPieces(int, bool king = false);
	int countPositions(int, int weight = 1);
	int countJumps(int);

};

#endif	/* BOARD_H */
