#include "board.h"

Board::Board(char *state = NULL){
	if(!state) state = "-1-1-1-1.1-1-1-1-.-1-1-1-1.--------.--------.2-2-2-2-.-2-2-2-2.2-2-2-2-";
	else {

	}
	int color;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = (state[place]=='-')? 0 : state[place]-'0';
			square[row][col] = new Square(color, row, col);
			if(color>2){
				square[row][col].color = color-5;
				square[row][col].king = 1;
			}
		}
	}
}

Board::print(){
	const char *shapes[2] = {"\033[44m ","\033[43m "};
	int color;
	for(row=7; row>=0; row--){
		for(column=7; column>=0; column--){
			color= square[row][column].color;
			cout<< shapes[color-1];
		}
		cout<<endl;
	}
}

Board::getLegalMoves(int color, Move *moves[]){
	Square myPieces[];
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col].color==color) myPieces[] = square[row][col];
		}
	}

	for(int i=0;i<sizeof(myPieces);i++){
			getJumps(piece, moves);
			if(!moves) getNonJumps(piece, &moves);		
	}
}

Board::getJumps(Square origin, Move *moves[]){
	Square *spots[];
	Square *spot;
	Move move;
	getAdjacents(origin,spots,0);
	for(i=0; i<sizeof(spots); i++){
		spot = spots[i];
		Square dest = getNextSquare(origin, spot);
		if(dest!=NULL && !dest.occupied){
				move = new Move(origin, dest, spot); //spot is the jumped variable!
				boardtmp = Board.copy();
				boardtmp = Board.makeMove(move);
				boardtmp.getJumps(dest, move.nextJumps);
				moves[] = move;
			}
		}
	}	
}

Board::getNonJumps(Square origin, Move *moves[]){
	Square *spots[];
	getAdjacents(origin, &spots, 1);
	for(i=0;i<sizeof(spots); i++){
		moves[] = new Move(origin, spots[i]);
	}
}

Board::getAdjacents(Square current, Square *spots[], bool available){ // Available = Valid and empty, Unavailable = Valid and other color (for jumps)
	row = current.x; col = current.y;
	Square s; //Temp variable to shorten code
	if(current.king || current.color == 1)){ //This is not a regular black (Go up)
		if(isValid(row+1, col-1)){
			s = square[row+1][square.col-1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = s;
		}
		if(isValid(row+1, col+1)){ 
			s = square[row+1][square.col+1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = s;
		}
	}
	if(current.king || current.color == 2)){ //This is not a regular red (Go down)
		if(isValid(row-1, col-1)){
			s = square[row-1][square.col-1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = s;
		}
		if(isValid(row-1, col+1)){ 
			s = square[row-1][square.col+1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = s;
		}
	}
}

Board::isValid(row, col){
	return (row>=0 && row<8 && col>=0 && col<8);
}

Board::getNextSquare(Square origin,Square jumped){
	int row = jumped.x + jumped.x-origin.x;
	int col = jumped.y + jumped.y-origin.y;
	return isValid(row, col)?square[row][col]:NULL;
}

Board::makeMove(Move move){
	Square origin = move.origin;
	square[move.origin.x][move.origin.y].occupied = 0;
	square[move.dest.x][move.dest.y].occupied = 1;
	square[move.dest.x][move.dest.y].color = square[move.origin.x][move.origin.y].color;
	if(move.jumped){
		square[move.jumped.x][move.jumped.y].occupied = 0;
		if(move.nextJumpChosen!=NULL)
			makeMove(move.nextJumpChosen);
	}
}

Board::copy(){
	String state = "";
	int place, color;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*9 + col;
			color = square[row][col].color;
			state[place] = (char) square[row][col].king? color+5: color;
		}
	}
	return new Board(state);	
}
