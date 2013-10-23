#include "board.h"

using namespace std;

Board::Board(char *state){
	if(!state) state = "1111.1111.1111.----.----.2222.2222.2222";
//	if(!state) state = "1111.1-11.1-11.1---.--1-.222-.2222.2222";
	
	
	int color, place;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			place = row*5 + col;
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
	//0=Black, 1=Red Piece, 2=Black Piece, 3=Red King, 4=Black King, 5=Red
	const char *shapes[6] = {"\033[40m     ","\033[40m \033[43m   \033[40m ","\033[40m \033[45m   \033[40m ", "\033[40m \033[43m K \033[40m ","\033[40m \033[45m K \033[40m ","\033[41m     "};
	int color;
	cout<<"     0    1    2    3    4    5    6    7"<<endl;
	for(int row=7; row>=0; row--){
		printRow(row%2);
		cout<<" "<<row<<" ";
		if(!(row%2)) cout<<shapes[5];
		for(int column=0; column<4; column++){
			if(square[row][column]->occupied){
				color = square[row][column]->color;
				if(square[row][column]->king) color+=2;
			}
			else color = 0;
			cout<< shapes[color];

			if(row%2 || column!=3) cout<<shapes[5];//Red square
		}
		cout<<"\033[0m "<<row<<endl;
		printRow(row%2);
	}
	cout<<"     0    1    2    3    4    5    6    7"<<endl<<endl<<endl;
}

void Board::printRow(int offset){ //Offset = 0 => Red First
	const char *shapes[2] = {"\033[41m     ", "\033[40m     "};
	cout<<"   ";
	for(int column=0;column<8;column++){
		cout<<shapes[offset];
		offset = 1-offset;
	}
	cout<<"\033[0m"<<endl;
}

bool Board::getLegalMoves(int color, vector<Move*> &moves){ //FALSE = GAME OVER
	vector<Square*> myPieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
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
	return !moves.empty();
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
	int col2 = (row%2) ? col-1 : col+1;
	Square* s;
	if(current->king || current->color == 1){ //This is not a regular black (Go up)
		if(isValid(row+1, col)){
			s = square[row+1][col];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
		if(isValid(row+1, col2)){ 
			s = square[row+1][col2];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
	}
	if(current->king || current->color == 2){ //This is not a regular red (Go down)
		if(isValid(row-1, col)){
			s = square[row-1][col];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
		if(isValid(row-1, col2)){ 
			s = square[row-1][col2];
			if((available && !s->occupied) || (!available && s->occupied && s->color!=current->color))
				spots.push_back(s);
		}
	}
}

bool Board::isValid(int row, int col){
	return (row>=0 && row<8 && col>=0 && col<4);
}

Square *Board::getNextSquare(Square *origin,Square *jumped){
	int off = !(jumped->x%2) ? 1 : -1;
	int row = jumped->x + jumped->x-origin->x;
	int col = jumped->y + jumped->y-origin->y + off;
	return isValid(row, col)?square[row][col]:NULL;
}

bool Board::makeMove(Move *move){ 
	Square *dest = square[move->dest->x][move->dest->y];
	dest->occupied = 1;
	dest->color = square[move->origin->x][move->origin->y]->color;
	dest->king = square[move->origin->x][move->origin->y]->king;
	emptySquare(square[move->origin->x][move->origin->y]);
	checkKing(dest);
	if(move->jumped != NULL){
		emptySquare(square[move->jumped->x][move->jumped->y]);
		if(move->nextJumpChosen!=NULL)
			return makeMove(move->nextJumpChosen);
	}
	return terminalTest(dest->color); //GAME OVER = FALSE
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

bool Board::terminalTest(int color){ //False = GAME OVER
	vector<Square*> myPieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color!=color) 
				return true;
		}
	}
	return false;
}

Board* Board::copy(){
	char *state = (char*) malloc(50);
	int place, place_int, color;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			place = row*5 + col;
			color = square[row][col]->color;
			place_int = square[row][col]->king? color+2: color;
			state[place] = square[row][col]->occupied?(place_int+'0'):'-';
		}
		state[place+1] = '.';
	}
	return new Board(state);	
}

Move *Board::getBestMove(int color, int depth, vector<Move*> &moves){ //probably combine with getLegalMoves
	Move *move, *bestMove;
	int utility;
	int bestUtility = -10000000;

	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		move = *it;
		utility = miniMaxVal(move, depth, true, color);
		move->value = utility;
		if(utility>bestUtility){
			bestUtility = utility;
			bestMove = move;
		}
	}
	return bestMove;
}

int Board::miniMaxVal(Move *move, int depth, bool turn, int color){ //Turn is true for MAX
	if(!(depth && terminalTest(color) )){
		return evaluateBoard(color);
	}
	if(!move->nextJumps.empty()){
		move->nextJumpChosen = getBestMove(color, depth, move->nextJumps);
	}
	Move *mv;
	int utility;
	int bestUtility = turn?-10000000:10000000;
	Board *boardtmp;

	boardtmp = copy();
	boardtmp->makeMove(move);
	vector<Move*> moves;
	boardtmp->getLegalMoves(3-color, moves); 
	
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		mv = *it;
		utility = boardtmp->miniMaxVal(mv, depth-1, !turn, 3-color);
		if((turn && utility>bestUtility)||(!turn && utility<bestUtility)){
			bestUtility = utility;
		}
	}
	return bestUtility;
}

int Board::evaluateBoard(int color){
	int pieceDiff = (countPieces(color) - countPieces(3-color));
	return pieceDiff*1000 + rand()%1000;
}

int Board::countPieces(int color){
	int count = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color) 
				count++;
		}
	}
	return count;
}
