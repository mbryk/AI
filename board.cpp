#include "board.h"

using namespace std;

Board::Board(char *state){
	//if(!state) state = "-1-1-1-1.1-1-1-1-.-1-1-1-1.--------.--------.2-2-2-2-.-2-2-2-2.2-2-2-2-";
	//if(!state) state = "---1-1-1.----1-1-.---1-1-1.--1-----.-1---2--.1-------.---2-2-2.2-2-2-2-";
	if(!state) state = "-----1-1.----1-1-.-------1.----2---.--------.--1-----.-------2.------2-";
	
	int color, place;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = (state[place]=='-')? 0 : state[place]-'0';
			square[row][col] = new Square(color, row, col);
			if(color>2){
				square[row][col]->color = color-2;
				square[row][col]->king = 1;
			}
		}
	}
}

void Board::print(){
	const char *shapes[3] = {"\033[0m--","\033[41m  ","\033[45m  "};
	int color;
	cout<<" 0 1 2 3 4 5 6 7"<<endl;
	for(int row=7; row>=0; row--){
		cout<<row;
		for(int column=0; column<8; column++){
			color = (square[row][column]->occupied)? square[row][column]->color : 0;
			cout<< shapes[color];
		}
		cout<<"\033[0m"<<endl;
	}
	cout<<endl<<endl;
}

void Board::getLegalMoves(int color, vector<Move*> &moves){
	vector<Square*> myPieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col]->occupied && square[row][col]->color==color) 
				myPieces.push_back(square[row][col]);
		}
	}

	for (vector<Square*>::iterator it = myPieces.begin() ; it != myPieces.end(); ++it){
			getJumps(*it, moves);
	}
	if(moves.empty()) 
		for (vector<Square*>::iterator it = myPieces.begin() ; it != myPieces.end(); ++it)
			getNonJumps(*it, moves);
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
				boardtmp->getJumps(boardtmp->square[dest->x][dest->y], move->nextJumps); //This is giving me an error, because dest is actually unoccupied.
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
	Square* s;
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

int Board::makeMove(Move *move){ 
	Square *dest = square[move->dest->x][move->dest->y];
	dest->occupied = 1;
	dest->color = square[move->origin->x][move->origin->y]->color;
	dest->king = square[move->origin->x][move->origin->y]->king;
	emptySquare(square[move->origin->x][move->origin->y]);
	checkKing(dest);
	if(move->jumped != NULL){
		emptySquare(square[move->jumped->x][move->jumped->y]);
		if(move->nextJumpChosen!=NULL)
			makeMove(move->nextJumpChosen);
	}
	return terminalTest(dest->color);
}

void Board::emptySquare(Square *sq){
	sq->occupied = 0;
	sq->color = 0;
	sq->king = false;
}

bool Board::checkKing(Square *sq){
	if(!sq->king){
		if( (!sq->x && sq->color==2) || (sq->x==7 && sq->color==1)){
			sq->king = true;
		}
	}
	return sq->king;
}

int Board::terminalTest(int color){
	vector<Square*> myPieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col]->occupied && square[row][col]->color!=color) 
				return 0;
		}
	}
	return color;
}

Board* Board::copy(){
	char *state = (char*) malloc(100);
	int place, place_int, color;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = square[row][col]->color;
			place_int = square[row][col]->king? color+2: color;
			state[place] = square[row][col]->occupied?(place_int+'0'):'-';
		}
		state[place+1] = '.';
	}
	return new Board(state);	
}
