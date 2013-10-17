#include "board.h"

using namespace std;

Board::Board(char *state){
	if(!state) state = "-1-1-1-1.1-1-1-1-.-1-1-1-1.--------.--------.2-2-2-2-.-2-2-2-2.2-2-2-2-";
	else {

	}
	int color, place;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = (state[place]=='-')? 0 : state[place]-'0';
			square[row][col] = new Square(color, row, col);
			if(color>2){
				square[row][col]->color = color-5;
				square[row][col]->king = 1;
			}
		}
	}
}

void Board::print(){
	const char *shapes[2] = {"\033[44m ","\033[43m "};
	int color;
	for(int row=7; row>=0; row--){
		for(int column=7; column>=0; column--){
			color= square[row][column]->color;
			cout<< shapes[color-1];
		}
		cout<<endl;
	}
}

void Board::getLegalMoves(int color, vector<Move*> &moves){
	vector<Square*> myPieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col]->color==color) 
				myPieces.push_back(square[row][col]);
		}
	}

	for (vector<Square*>::iterator it = myPieces.begin() ; it != myPieces.end(); ++it){
			getJumps(*it, moves);
			if(moves.empty()) getNonJumps(*it, moves);		
	}
}

void Board::getJumps(Square *origin, vector<Move*> &moves){
	vector<Square*> spots;

	Square *spot;
	Move *move;
	getAdjacents(origin,spots,0);
	for (vector<Square*>::iterator it = spots.begin() ; it != spots.end(); ++it){
		spot = *it;
		Square *dest = getNextSquare(origin, spot);
		if(dest!=NULL && !dest->occupied){
				move = new Move(origin, dest);
				move->jumped = spot;
				Board *boardtmp = copy();
				boardtmp->makeMove(move);
				boardtmp->getJumps(dest, move->nextJumps);
				moves.push_back(move);
		}
	}	
}

void Board::getNonJumps(Square *origin, vector<Move*> &moves){
	vector<Square*> spots;
	Move *move;
	Square *spot;
	getAdjacents(origin, spots, 1);
	for (vector<Square*>::iterator it = spots.begin() ; it != spots.end(); ++it){
		spot = *it;
		move = new Move(origin, spot);
		moves.push_back(move);

	}
}

void Board::getAdjacents(Square *current, vector<Square*> &spots, bool available){ // Available = Valid and empty, Unavailable = Valid and other color (for jumps)
	int row = current->x; int col = current->y;
	Square* s; //Temp variable to shorten code
	if(current->king==1 || current->color == 1){ //This is not a regular black (Go up)
		if(isValid(row+1, col-1)){
			s = square[row+1][col-1];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
		if(isValid(row+1, col+1)){ 
			s = square[row+1][col+1];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
	}
	if(current->king==1 || current->color == 2){ //This is not a regular red (Go down)
		if(isValid(row-1, col-1)){
			s = square[row-1][col-1];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
		if(isValid(row-1, col+1)){ 
			s = square[row-1][col+1];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
	}
}

bool Board::isValid(int row, int col){
	return (row>=0 && row<8 && col>=0 && col<8);
}

Square *Board::getNextSquare(Square *origin,Square *jumped){
	int row = jumped->x + jumped->x-origin->x;
	int col = jumped->y + jumped->y-origin->y;
	return isValid(row, col)?square[row][col]:NULL;
}

bool Board::makeMove(Move *move){
	square[move->origin->x][move->origin->y]->occupied = 0;
	square[move->dest->x][move->dest->y]->occupied = 1;
	square[move->dest->x][move->dest->y]->color = square[move->origin->x][move->origin->y]->color;
	if(move->jumped != NULL){
		square[move->jumped->x][move->jumped->y]->occupied = 0;
		if(move->nextJumpChosen!=NULL)
			makeMove(move->nextJumpChosen);
	}
	return 1;
}

Board* Board::copy(){
	char *state = (char*) malloc(100);
	int place, color;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = square[row][col]->color;
			state[place] = (char) square[row][col]->king? color+5: color;
		}
	}
	return new Board(state);	
}
