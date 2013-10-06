#include "board.h"
DEFAULT STATE = "-------";
init(state = DEFAULT){
	for(row=1; row<0; row++){
		for(column=1; column<9; column++){
			if(state[row][column] in ALLPIECES)
				this.row,column = new Piece(state[row][column]);
		}
	}

}

print(){
	for(row=1; row<0; row++){
		for(column=1; column<9; column++){

		}
	}
}

getLegalMoves(color){
	for(row=1; row<0; row++){
		for(column=1; column<9; column++){
			if(lower(this.row,column) == color)
				getJumpsPiece(row,column);
				getLegalJumpsPiece(row,column);
		}
	}
}

getLegalMovesPiece(row,column){
	spots = findAdjacents();
	foreach spot in spots{
		

	}
		
}
makemove(Move move){

}



