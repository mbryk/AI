#ifndef BOARD_H
#define	BOARD_H
#include "square.h"
#include "move.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string.h>
#include "time.h"
#include "sys/time.h"

using namespace std;

class Board{

public:
	Board(const char*);
	bool debugPrint;
	
	struct timeval t_start;
	struct timeval t_now;
	double t_lim;
	int color;
	int hnum;

	void print();
	bool getLegalMoves(int, vector<Move*>&);
	bool makeMove(Move*); // returns "Game Over"
	Move *getBestMove(int, vector<Move*>&, bool&);
	double tdiff();
	void deleteMoves(vector<Move*>&);
private:
	Square *square[8][4];
	bool end_section;
	void printRow(int);
	void getAdjacents(Square*, vector<Square*>&, bool);
	void getJumps(Square*, vector<Move*>&);
	void simplify(vector<Move*>&);
	void getNonJumps(Square*, vector<Move*>&);
	bool isValid(int, int);
	Square *getNextSquare(Square*,Square*);
	void emptySquare(Square*);
	bool checkKing(Square*);
	bool terminalTest(int);
	Board *copy();
	void deleteBoard(Board*);
	int miniMaxVal(Move*, int, bool, int, int);
	int evaluateBoard(int, bool term = false);
	int h1();
	int h2();
	int h3(int,bool);
	int h3begin(int, int);
	int h3end(int);
	int distances(int,int,int);
	int countPieces(int, bool king = false);
	int countPositions(int, int weight = 1);
	int countJumps(int);

};

#endif	/* BOARD_H */
