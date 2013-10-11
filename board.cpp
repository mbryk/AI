#include "board.h"

DEFAULT STATE = "-r-r-r-r.r-r-r-r-.-r-r-r-r.--------.--------.b-b-b-b-.-b-b-b-b.b-b-b-b-";
Board::Board(state = DEFAULT){
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*8 + col;
			square[row][col] = new Square(row, col, state[place]);
		}
	}
}

Board::print(){
	for(row=7; row>=0; row--){
		for(column=7; column>=0; column--){
			color= square[row][column].color;
			cout<< format[color]; //Colors is a key value array with "r"=>red, "b"=>black, "-"=>white
		}
		cout<<endl;
	}
}

Board::getLegalMoves(color){
	Square myPieces[];
	Move *moves[];
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col].color==color) myPieces[] = square[row][col];
		}
	}

	for(int i=0;i<sizeof(myPieces);i++){
		getLegalMovesPiece(myPieces[i], &moves);
	}
}

Board::getLegalMovesPiece(Square piece, bool only_jumps = false, Move *moves[]){
	getJumps(piece, &moves);
	if(!(moves || only_jumps)) getNonJumps(piece, &moves);		
}

Board::getAdjacents(int row, int col, bool available, Square *spots[]){ // Available = Valid and empty, Unavailable = Valid and other color
	Square current = square[row][col];
	if(current.king || current.color == "r")){ //This is not a regular black (Go up)
		if(isValid(row+1, col-1)){
			Square s = square[row+1][square.col-1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = square[row+1][square.col-1];
		}
		if(isValid(row+1, col+1)){ 
			Square s = square[row+1][square.col+1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = square[row+1][square.col+1];
		}
	}
	if(current.king || current.color == "b")){ //This is not a regular red (Go down)
		if(isValid(row-1, col-1)){
			Square s = square[row-1][square.col-1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = square[row-1][square.col-1];
		}
		if(isValid(row-1, col+1)){ 
			Square s = square[row-1][square.col+1];
			if((available && !s.occupied) || (!available && s.occupied && s.color!=current.color))
				spots[] = square[row-1][square.col+1];
		}
	}
}

Board::isValid(row, col){
	return (row>=0 && row<8 && col>=0 && col<8);
}

Board::getJumps(Square origin, Move *moves[]){
	Square *spots[];
	Square *spot;
	getAdjacents(origin.x, origin.y,0,&spots);
	for(i=0; i<sizeof(spots); i++){
		spot = spots[i];
		Square dest = getNextSquare(origin, spot);
		if(dest!=NULL && !dest.occupied){
				move = new Move(origin, spot);
				boardtmp = Board.copy();
				boardtmp = Board.makeMove(move);
				boardtmp.getLegalMovesPiece(move.destination, 1, &move.nextJump);
				moves[] = move;
			}
		}

	}	
}

Board::getNonJumps(Square origin, Move *moves[]){
	Square *spots[];
	getAdjacents(origin.x, origin.y, 1, &spots);
	for(i=0;i<sizeof(spots); i++){
		moves[] = new Move(origin, spot);
	}
}

Board::getNextSquare(Square origin,Square jumped){
	int row = jumped.x + jumped.x-origin.x;
	int col = jumped.y + jumped.y-origin.y;
	return isValid(row, col)?square[row][col]:NULL;
}

Board::makeMove(Move move){

}

Board::copy(){
	String state = "";
	char color;
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			place = row*8 + col;
			color = square[row][col].color;
			state[place] = square[row][col].is_king? upper(color): color;
		}
	}
	return new Board(state);	
}
