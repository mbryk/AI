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
			cout<< colors[lower(color)] << format[color]; //Colors is a key value array with "r"=>red, "b"=>black, "-"=>white
		}
		cout<<endl;
	}
}

Board::getLegalMoves(color){
	Square myPieces[];
	Move moves[];
	for(int row=0; row<8; row++){
		for(int col=0; col<8; col++){
			if(square[row][col].color==color) myPieces[] = square[row][col];
		}
	}

	for(int i=0;i<sizeof(myPieces);i++){
		moves = getLegalMovesPiece(myPieces[i]);
	}
}

Board::getLegalMovesPiece(Square piece, bool only_jumps = false){
	Square spots[];
	spots = getAdjacents(row,column);
	Move moves[];
	moves = getJumps(spots, piece);
	if(!(moves || only_jumps)) moves = getNonJumps(spots, piece);		

	return moves;
}

Board::getAdjacents(Square square){
	Square spots[];
	if(square.king || square.color == "r")) //This is not a regular black (Go up)
	{
		if(isValid(square.row+1, square.column-1)) 
			spots[] = new Square(square.row+1, square.column-1);
		if(isValid(square.row+1, square.column+1)) 
			spots[] = new Square(square.row+1, square.column+1);
	}
	if(square.king || square.color == "b")) //This is not a regular red (Go down)
	{
		if(isValid(square.row-1, square.column-1)) 
			spots[] = new Square(square.row-1, square.column-1);
		if(isValid(square.row-1, square.column+1)) 
			spots[] = new Square(square.row+1, square.column+1);
	}
	return spots;
}

Board::isValid(row, col){
	return (row>=0 && row<8 && col>=0 && col<8);
}

Board::getJumps(Square spots[], Square origin){
	Move moves[];
	foreach(spot in spots){
		if(spot.occupied && spot.color != origin.color){
			Square dest = getNextSquare(origin, spot);
			if(!dest.occupied){
				move = new Move(origin, spot);
				boardtmp = Board.copy();
				boardtmp = Board.makeMove(move);
				move.nextJump = boardtmp.getLegalMovesPiece(move.destination, 1);
				moves[] = move;
			}
		}

	}
	return moves;
	
}

Board::getNonJumps(Square spots[], Square origin){
	Move moves[];
	foreach(spot in spots){
		if(!spot.occupied){
			moves[] = new Move(origin, spot);
		}
	}
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
