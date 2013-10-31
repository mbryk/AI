#include "board.h"
#define MININF -10000000
#define POSINF 10000000
using namespace std;

Board::Board(char *state){
	if(!state) state = "1111.1111.1111.----.----.2222.2222.2222";
	//if(!state) state = "----.----.----.----.----.----.--1-.--22";
	
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

	end_section = false;
}

void Board::print(){
	//0=Black, 1=Red Piece, 2=Black Piece, 3=Red King, 4=Black King, 5=Red
	const char *shapes[6] = {"\033[40m     ","\033[40m \033[43m   \033[40m ","\033[40m \033[45m   \033[40m ", "\033[40m \033[43;1m K \033[40m ","\033[40m \033[45;1m K \033[40m ","\033[41m     "};
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
		if(countPieces(1,true)==countPieces(1) && countPieces(2,true)==countPieces(2))
			end_section = true;
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
	Board *board;
	board = new Board(state);
	board->debugPrint = debugPrint;
	board->end_section = end_section;
	return board;	
}

Move *Board::getBestMove(int color, int depth, vector<Move*> &moves, int hnum){ //probably combine with getLegalMoves
	Move *move, *bestMove;
	int utility;
	int i = 1;
	int bestUtility = MININF;
	if(debugPrint) cout<<"DEPTH "<<depth<<endl;
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		if(debugPrint) cout<<"\tMove "<<i++<<": ";
		move = *it;
		utility = miniMaxVal(move, depth, true, color, MININF-1, POSINF, hnum);
		move->value = utility;
		if(utility>bestUtility){
			bestUtility = utility;
			bestMove = move;
		}
	}
	return bestMove;
}

int Board::miniMaxVal(Move *move, int depth, bool turn, int color, int alpha, int beta, int hnum){ //Turn is true for MAX
	if(!move->nextJumps.empty()){
		if(debugPrint) {
			debugPrint = false;
			move->nextJumpChosen = getBestMove(color, depth, move->nextJumps, hnum);
			debugPrint = true;
			cout<<endl<<"bestMoveChosen: ";
			move->nextJumpChosen->print();
			cout<<endl;
		} else move->nextJumpChosen = getBestMove(color, depth, move->nextJumps, hnum);
	}
	
	Board *boardtmp;
	boardtmp = copy();
	boardtmp->makeMove(move);
	if(!(depth-1) || !boardtmp->terminalTest(color)){
		if(debugPrint){
			int val = boardtmp->evaluateBoard(color, hnum);
			move->print();
			cout<<"--Bval: "<<val<<endl;
			return val;
		}else {
			return boardtmp->evaluateBoard(color, hnum);
		}
	}

	Move *mv;
	int utility;
	int bestUtility = turn?POSINF:MININF;
	vector<Move*> moves;

	if(turn) boardtmp->getLegalMoves(3-color, moves); 
	else boardtmp->getLegalMoves(color, moves);
	if(debugPrint) cout<<endl<<"||||||||||||"<<endl;
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		mv = *it;
		utility = boardtmp->miniMaxVal(mv, depth-1, !turn, color, alpha, beta, hnum);
		if(turn){
			beta = min(beta, utility);
			if(beta<=alpha) { if(debugPrint) {cout<<"PRUNED1 ----(";move->print();cout<<")";} return alpha;}
		} else {
			alpha = max(alpha, utility);
			if(alpha>=beta) { if(debugPrint) {cout<<"PRUNED2 ----(";move->print();cout<<")";} return beta;}
		}
		if((turn && utility<bestUtility) || (!turn && utility>bestUtility)){
			bestUtility = utility;
		}
	}
	if(debugPrint){
		cout<<"---------"<<endl;
		move->print();
		cout<<" ---U="<<bestUtility<<endl;
	}

	return bestUtility;
}

int Board::evaluateBoard(int color, int hnum){
	int val;
	switch(hnum){
		case 2:
			val = h2(color);
			break;
		case 3:
			val = h3(color);
			break;
		default:
			val = h1(color);
			break;
	}
	return val;
}

int Board::h1(int color){
	int pieceDiff = (countPieces(color) - countPieces(3-color));
	return pieceDiff*1000 + rand()%200;
}

int Board::h2(int color){
	int pieces = countPieces(color) - countPieces(3-color);
	int kings = countPieces(color, true) - countPieces(3-color, true);
	//int pos = countPositions(color) - countPositions(3-color);
	int jumps = countJumps(color) - countJumps(3-color);
	return pieces*500 + kings*300 + jumps*200 + rand()%30;
}

int Board::countPieces(int color, bool king){
	int count = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color){ 
				if(king && !square[row][col]->king) continue;
				count++;
			}
		}
	}
	return count;
}

int Board::countJumps(int color){
	vector<Move*> moves;
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
	return moves.size();
}

int Board::countPositions(int color, int weight){
	int count=0;
	for(int row=0; row<8; row++){
		int a = (color==1)?row:7-row;
		a*=weight;
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color){
				count += a;
			}
		}
	}
	return count;
}
int Board::h3(int color){
	if(end_section) return (h3end(color)-h3end(3-color));
	else return (h3begin(color)-h3begin(3-color));
}

int Board::h3begin(int color){
	int a, col;
	int count = 0;
	for(int row=0; row<8; row++){
		a = (color==1)?row:7-row; //rows from bottom
		for(col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color){ 
				//ADD PIECE VAL
				if(square[row][col]->king) count+=10;
				//ADD POS VAL
				else if(a<4) count+=5;
				else count+=7;

				//ADD SIDES
				if(!col || col==3) count+=3;
			}
		}
	}
	a = color==1?0:7; int br = 0;
	for(col=0;col<4;col++) {
		if(square[a][col]->occupied) continue; 
		if(col==3) count+=10;
	}
	return count*100 + rand()%20;
}
int Board::h3end(int color){
	int dist = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color){ 
				dist += distances(row, col, 3-color);
			}
		}
	}
	int k = countPieces(color); int k2 = countPieces(3-color);
	int rn = rand()%20;
	if(k==k2) return rn;
	if(k<k2) return dist*-100 + rn;
	return dist*100 + rn;
}

int Board::distances(int r, int c, int color){
	int dist = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->occupied && square[row][col]->color==color){ 
				dist += abs(row-r) + abs(col-c)/2;
			}
		}
	}	
}