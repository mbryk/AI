#include "board.h"
#define MININF -1000000000
#define POSINF 1000000000
using namespace std;

Board::Board(const char* state){
	char s[40];
	strncpy( s, state,39 ); // To convert char* to char[] for the sake of strtok. Lots of c-style processing
	int color, col;
	int row = 0;
	char *r = strtok(s," .");

	while(row<8){
		for(col=0; col<4; col++){
			color = r[col]-'0';
			if(color<0 || color>4) color = 0;
			square[row][col] = new Square(color, row, col);
			if(color>2){
				square[row][col]->color = color-2;
				square[row][col]->king = 1;
			}
		}
		row++;
		r = strtok(NULL, " .");
	}
	end_section = (countPieces(0)<9);
	draw = false;
}

void Board::print(){
	//0=Black, 1=Yellow Piece, 2=Black Piece, 3=Yellow King, 4=Black King, 5=Red
	const char *shapes[6] = {"\033[40m     ","\033[40m \033[43m   \033[40m ","\033[40m \033[45m   \033[40m ", "\033[40m \033[30;43;1m K \033[40m ","\033[40m \033[37;45;1m K \033[40m ","\033[41m     "};
	int color;
	cout<<"     0    1    2    3    4    5    6    7"<<endl;
	for(int row=7; row>=0; row--){
		printRow(row%2);
		cout<<" "<<row<<" ";
		if(!(row%2)) cout<<shapes[5];
		for(int column=0; column<4; column++){
			color = square[row][column]->color;
			if(square[row][column]->king) color+=2;
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
			if(square[row][col]->color==color) 
				myPieces.push_back(square[row][col]);
		}
	}

	for (vector<Square*>::iterator it = myPieces.begin() ; it != myPieces.end(); ++it){
			getJumps(*it, moves);
	}
	if(moves.empty()) 
		for (vector<Square*>::iterator it = myPieces.begin() ; it != myPieces.end(); ++it)
			getNonJumps(*it, moves);
	myPieces.clear();
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
		if(dest!=NULL && dest->color==0){//On the board and unoccupied
				move = new Move(origin, dest);
				move->jumped = spot;
				Board *boardtmp = copy();
				if(boardtmp->makeMove(move)){
					if(origin->king || !boardtmp->square[dest->x][dest->y]->king){
						boardtmp->getJumps(boardtmp->square[dest->x][dest->y], move->nextJumps); //This is giving me an error, because dest is actually unoccupied.
						simplify(move->nextJumps);					
					}
				}
				deleteBoard(boardtmp);
				moves.push_back(move);
		}
	}	
	spots.clear();
}
void Board::simplify(vector<Move*> &moves){
	Square *tmp;
	Move *move;
	for(vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it){
		move = *it;
		tmp = move->origin;
		move->origin = square[tmp->x][tmp->y];
		tmp = move->dest;
		move->dest = square[tmp->x][tmp->y];
		tmp = move->jumped;
		move->jumped = square[tmp->x][tmp->y];
		if(!move->nextJumps.empty()){
			simplify(move->nextJumps);
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
	spots.clear();
}

void Board::getAdjacents(Square *current, vector<Square*> &spots, bool available){ // Available = Valid and empty, Unavailable = Valid and other color (for jumps)
	int row = current->x; int col = current->y; 
	int col2 = (row%2) ? col-1 : col+1;
	Square* s;
	if(current->king || current->color == 1){ //This is not a regular black (Go up)
		if(isValid(row+1, col)){
			s = square[row+1][col];
			if((available && !s->color) || (!available && s->color==3-current->color))
				spots.push_back(s);
		}
		if(isValid(row+1, col2)){ 
			s = square[row+1][col2];
			if((available && !s->color) || (!available && s->color==3-current->color))
				spots.push_back(s);
		}
	}
	if(current->king || current->color == 2){ //This is not a regular red (Go down)
		if(isValid(row-1, col)){
			s = square[row-1][col];
			if((available && !s->color) || (!available && s->color==3-current->color))
				spots.push_back(s);
		}
		if(isValid(row-1, col2)){ 
			s = square[row-1][col2];
			if((available && !s->color) || (!available && s->color==3-current->color))
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

bool Board::makeMove(Move *move, bool checkDraw){ 
	Square *dest = square[move->dest->x][move->dest->y];
	dest->color = square[move->origin->x][move->origin->y]->color;
	dest->king = square[move->origin->x][move->origin->y]->king;
	emptySquare(square[move->origin->x][move->origin->y]);
	if(!dest->king) checkKing(dest);
	if(move->jumped != NULL){
		if(!end_section && countPieces(0)<9) end_section = true;
		emptySquare(square[move->jumped->x][move->jumped->y]);
		if(move->nextJumpChosen!=NULL){
			int m = makeMove(move->nextJumpChosen);
			if(end_section && checkDraw) isDrawingBoard();
			return m;
		} else {
			if(end_section && checkDraw) isDrawingBoard();
		}
		return terminalTest(dest->color); //Check terminal test on winning color
	}

	if(end_section && checkDraw) isDrawingBoard();
	return true; //GAME OVER = FALSE
}

void Board::emptySquare(Square *sq){
	sq->color = 0;
	sq->king = false;
}

bool Board::checkKing(Square *sq){
	if( (!sq->x && sq->color==2) || (sq->x==7 && sq->color==1)){
		sq->king = true;
		if(!end_section && countPieces(1,true)>(countPieces(1)-2) && countPieces(2,true)==countPieces(2)-2)
			end_section = true;
		return true;
	}
	return false;
}

bool Board::terminalTest(int color){ //False = GAME OVER
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color==3-color) 
				return true;
		}
	}
	return false;
}

void Board::isDrawingBoard(){ //Offer a draw if both players have only kings, less than 4, and an equal amount, with no jumps available on the next move.
	draw = true;
	int count[2] = {0,0};
	vector<Square*> pieces;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color>0){ //Occupied
				if(square[row][col]->king) count[square[row][col]->color-1]++;
				else {draw = false; break;}
				if(square[row][col]->color==3-color)
					pieces.push_back(square[row][col]);
			}
		}
		if(!draw) break;
	}

	if(draw && count[0]==count[1] && count[0]<4){
			vector<Square*> spots;
			Square *spot, *piece, *dest;
			for (vector<Square*>::iterator it1 = pieces.begin() ; it1 != pieces.end(); ++it1){
				piece = *it1;
				getAdjacents(piece,spots,0);
				if(spots.size()>0){
					for (vector<Square*>::iterator it2 = spots.begin() ; it2 != spots.end(); ++it2){
						spot = *it2;
						dest = getNextSquare(piece, spot);
						if(dest!=NULL && dest->color==0)
							draw = false;
					}	
				}
				spots.clear();
			}
	} else draw = false;
	pieces.clear();
}

Board* Board::copy(){
	char *state = (char*) malloc(50);
	int place, place_int, c;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			place = row*5 + col;
			c = square[row][col]->color;
			place_int = square[row][col]->king? c+2: c;
			state[place] = place_int?place_int+'0':'-';
		}
		state[place+1] = '.';
	}
	Board *board;
	board = new Board(state);
	board->hnum = hnum;
	board->end_section = end_section;
	board->color = color;
	board->t_lim = t_lim;
	board->t_start = t_start;
	return board;	
}

void Board::deleteBoard(Board* board){
	int col;
	for(int row=0; row<8; row++)
		for(int col=0; col<4; col++)
			delete board->square[row][col];
	delete board;
}

void Board::deleteMoves(vector<Move*> &moves){
	Move *move;
	while(!moves.empty()) {
        move = moves.back();
        if(!move->nextJumps.empty()) 
        	deleteMoves(move->nextJumps);
        delete move;
        moves.pop_back();
    }
    moves.clear();
}

Move *Board::getBestMove(int depth, vector<Move*> &moves, bool &no_options){
	Move *move, *bestMove, *tempMove;
	if(moves.size()==1){
		no_options = true;
		move = moves.at(0);
		if(!move->nextJumps.empty()){
			tempMove = move->nextJumpChosen;
			move->nextJumpChosen = NULL;
			Board *boardtmp;
			boardtmp = copy();
			if(boardtmp->makeMove(move)){
				no_options = false;
				if((move->nextJumpChosen =boardtmp->getBestMove(depth, move->nextJumps, no_options))==NULL){ //Now, if there is more than one option, no_options becomes false again
					move->nextJumpChosen = tempMove;
					deleteBoard(boardtmp);
					return NULL;
				}
			}
			deleteBoard(boardtmp);
		}
		return move;
	}

	int utility;
	int i = 1;
	int bestUtility = MININF;

	try{
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		i++;
		move = *it;
		utility = miniMaxVal(move, depth, true, MININF-1, POSINF+1);
		if(utility>bestUtility){
			bestUtility = utility;
			bestMove = move;
		}
	}
	return bestMove;
	} catch(int cat){ return NULL;}
}

double Board::tdiff(){
	double usecs = difftime(t_now.tv_usec,t_start.tv_usec)/1000000;
	double secs = difftime(t_now.tv_sec,t_start.tv_sec);
	if(usecs<0) {secs--;usecs = 1+usecs;}
	return secs+usecs;
}

int Board::miniMaxVal(Move *move, int depth, bool turn, int alpha, int beta){ //Turn is true for MAX
	gettimeofday(&t_now, NULL);
	if(tdiff()>(t_lim-.25)) throw 2;
	int trn;

	Board *boardtmp;
	Move *tempMove;
	bool f = false;
	boardtmp = copy();
	if(!move->nextJumps.empty()){
		if(boardtmp->makeMove(move))
			if((tempMove=boardtmp->getBestMove(depth, move->nextJumps, f))!=NULL)
				move->nextJumpChosen = tempMove;
			else {
				deleteBoard(boardtmp);
				throw 2;
			}
		boardtmp = copy();
	}
	bool t;
	// If turn, then MAX(color) just moved. Check terminalTest() on winning color
	if( (!(t = boardtmp->makeMove(move))) || (!(depth-1))) { 
		trn = turn?(3-color):color;
		int val = boardtmp->evaluateBoard(depth, !t, trn); //t=true if terminal. trn=color of next persons turn
		deleteBoard(boardtmp);
		return val;
	}

	Move *mv;
	int utility;
	int bestUtility = turn?POSINF:MININF;
	vector<Move*> moves;

	if(turn) boardtmp->getLegalMoves((3-color), moves); 
	else boardtmp->getLegalMoves(color, moves);

	try{

	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		mv = *it;
		utility = boardtmp->miniMaxVal(mv, depth-1, !turn, alpha, beta);
		if(turn){
			beta = min(beta, utility);
			if(beta<=alpha) { 
				deleteMoves(moves);deleteBoard(boardtmp);
				return alpha;
			}
		} else {
			alpha = max(alpha, utility);
			if(alpha>=beta) { 
				deleteMoves(moves);deleteBoard(boardtmp);				
				return beta;
			}
		}
		if((turn && utility<bestUtility) || (!turn && utility>bestUtility)){
			bestUtility = utility; //If (turn), then all of these utilities returned are from MIN's turn. Minimize them.
		}
	}
	deleteMoves(moves);
	deleteBoard(boardtmp);
	return bestUtility;

	} catch (int cat){
		deleteMoves(moves);
		deleteBoard(boardtmp);
		throw 2;
	}

}

int Board::evaluateBoard(int depth, int turn, bool term){
	int val;
	switch(hnum){
		case 2:
			val = h2(depth, turn, term);
			break;
		default:
			val = h1();
			break;
	}
	return val;
}

int Board::h1(){
	int pieceDiff = countPieces(color) - countPieces(3-color);
	int kings = countPieces(color, true) - countPieces(3-color, true);
	//int pos = countPositions(color) - countPositions(3-color);
	int jumps = countJumps(color) - countJumps(3-color);
	return pieceDiff*1000 + kings*500 + jumps*100 + rand()%30;
}


int Board::h2(int depth, int turn, bool term){
	int val = h2each(color,turn)/2-h2each(3-color, turn)/2;

	if(term) val += turn?(20-depth)*-10:(20-depth)*10; //For you dying, smaller depth = worse. For him dying, smaller depth = GOOD!
	
	return val;
}

int Board::h2each(int clr, int turn){
	int a, col, count;
	int distScore, tradeScore, pieceWeight,kingWeight,sideBonus,turnBonus,backRowBonus,middleBonus,depthSub, kingCornerBonus;
	int pieces = 0; /* 0 to 24 pieces. Creates a utility*/
	int pieceDiff = 0;
	int dist = 0;
	
	int piecesScore = 0; /* 0 to 100 */
	int rowScore = 0; /* 0 to 100 */
	int bonuses = 0;

	pieceWeight = 3;
	kingWeight = 2;

	turnBonus = 20;
	sideBonus = 30;
	backRowBonus = 15;
	middleBonus = 15;
	kingCornerBonus = 30;
	
	for(int row=0; row<8; row++){
		a = (clr==1)?row:7-row; //rows from bottom
		for(col=0; col<4; col++){
			if(square[row][col]->color==clr){ 
				pieces++;
				pieceDiff++;

				//Add Piece Bonus
				piecesScore += pieceWeight;
				//Add King Bonus
				if(square[row][col]->king) piecesScore+= kingWeight;
				
				//ADD Row Bonus
				if(a>3 && !square[row][col]->king) rowScore += a*4;

				//Add to distances
				dist += distances(row, col, 3-clr); //Distance from this space to other player's kings

				//ADD other bonuses
				if((!col && row%2==1) || (col==3 && row%2==0)) bonuses += sideBonus;
			}
			else if(square[row][col]->color==3-clr){
				pieces++;
				pieceDiff--;
			}
		}
	}

	if((pieceDiff<0 && color!=clr) || (pieceDiff>0 && color==clr)){
		tradeScore = pieces*-5;	
		distScore = dist*-4;
	} 
	else {
		tradeScore = pieces*5;
		distScore = dist*4;
	}

	//Back Row Bonus (Rarity of all 4 of other team in back row can be ignored for !end_section)
	a = color==1?0:7; int br = 0;
	for(col=0;col<4;col++) {
		if(!square[a][col]->color) br++;
	}
	if(br<2) bonuses += (2-br)*20;

	//Middle Bonus
	if(square[3][2]->color==clr) bonuses+=middleBonus;
	if(square[4][1]->color==clr) bonuses+=middleBonus;
	
	//Double Corner Bonus
	if(pieceDiff<0){
		if(square[0][0]->color==clr || square[1][0]->color==clr) bonuses+=kingCornerBonus;
		if(square[6][3]->color==clr || square[7][3]->color==clr) bonuses+=kingCornerBonus;
	}
	//TURN Bonus
	if(turn==clr) bonuses+=turnBonus;
	
	if(end_section) count = piecesScore*10000000 + tradeScore*100000 + distScore*100000 + rowScore*10000 + bonuses*10 + rand()%10;
	else count= piecesScore*10000000 + rowScore*100000 + tradeScore*1000 + bonuses*10 + rand()%10;
	
	return count;
}

int Board::countPieces(int color, bool king){
	int count = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color==color){ 
				if((king && !square[row][col]->king) || !color) continue;
				count++;
			}
			else if(color==0) count++;
		}
	}
	return count;
}

int Board::countJumps(int color){
	vector<Move*> moves;
	vector<Square*> myPieces;

	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color==color) 
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
			if(square[row][col]->color==color){
				count += a;
			}
		}
	}
	return count;
}
int Board::distances(int r, int c, int color){
	int dist = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color==color && square[row][col]->king){ 
				dist += abs(col-c) - abs(row-r);
				dist = dist>0? dist:0;
				dist += abs(row-r);
			}
		}
	}
	return dist;	
}