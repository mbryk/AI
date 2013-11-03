#include "board.h"
#define MININF -10000000
#define POSINF 10000000
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
		/*if((r = strtok(NULL, " ."))==NULL){
			cerr<<"Invalid Board State"<<endl; exit(-1);
		}*/
		r = strtok(NULL, " .");
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
					boardtmp->getJumps(boardtmp->square[dest->x][dest->y], move->nextJumps); //This is giving me an error, because dest is actually unoccupied.
					simplify(move->nextJumps);					
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

bool Board::makeMove(Move *move){ 
	Square *dest = square[move->dest->x][move->dest->y];
	dest->color = square[move->origin->x][move->origin->y]->color;
	dest->king = square[move->origin->x][move->origin->y]->king;
	emptySquare(square[move->origin->x][move->origin->y]);
	if(!dest->king) checkKing(dest);
	if(move->jumped != NULL){
		if(!end_section && countPieces(0)<11) end_section = true;
		emptySquare(square[move->jumped->x][move->jumped->y]);
		if(move->nextJumpChosen!=NULL)
			return makeMove(move->nextJumpChosen);
		return terminalTest(dest->color); //Check terminal test on winning color
	}
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
	board->debugPrint = debugPrint;
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
	Move *move, *bestMove;
	int utility;
	int i = 1;
	int bestUtility = MININF;
	if(debugPrint) cout<<"DEPTH "<<depth<<endl;
	if(moves.size()==1){
		no_options = true;
		move = moves.at(0);
		if(!move->nextJumps.empty()){
			Board *boardtmp;
			boardtmp = copy();
			if(boardtmp->makeMove(move)){
				bool f = false;
				move->nextJumpChosen = getBestMove(depth, move->nextJumps, f);
			}
			deleteBoard(boardtmp);
		}
		return move;
	}

	try{
	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){

		if(debugPrint) cout<<"\tMove "<<i<<": ";
		i++;
		move = *it;
		utility = miniMaxVal(move, depth, true, MININF-1, POSINF+1);
		move->value = utility;
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
	if(tdiff()>(t_lim-.1)) throw 2;

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
		int val = boardtmp->evaluateBoard(depth, !t);
		deleteBoard(boardtmp);
		if(debugPrint){
			move->print();
			cout<<"--Bval: "<<val<<endl;
		}
		return val;
	}

	Move *mv;
	int utility;
	int bestUtility = turn?POSINF:MININF;
	vector<Move*> moves;

	if(turn) boardtmp->getLegalMoves((3-color), moves); 
	else boardtmp->getLegalMoves(color, moves);
	if(debugPrint) cout<<endl<<"||||||||||||"<<endl;

	try{

	for (vector<Move*>::iterator it = moves.begin() ; it != moves.end(); ++it){
		mv = *it;
		utility = boardtmp->miniMaxVal(mv, depth-1, !turn, alpha, beta);
		if(turn){
			beta = min(beta, utility);
			if(beta<=alpha) { 
				if(debugPrint) {cout<<"PRUNED1 ----(";move->print();cout<<")";} 
				deleteMoves(moves);deleteBoard(boardtmp);
				return alpha;
			}
		} else {
			alpha = max(alpha, utility);
			if(alpha>=beta) { 
				if(debugPrint) {cout<<"PRUNED2 ----(";move->print();cout<<")";} 
				deleteMoves(moves);deleteBoard(boardtmp);				
				return beta;
			}
		}
		if((turn && utility<bestUtility) || (!turn && utility>bestUtility)){
			bestUtility = utility; //If (turn), then all of these utilities returned are from MIN's turn. Minimize them.
		}
	}
	if(debugPrint){
		cout<<"---------"<<endl;
		move->print();
		cout<<" ---U="<<bestUtility<<endl;
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

int Board::evaluateBoard(int depth, bool term){
	int val;
	switch(hnum){
		case 2:
			val = h2();
			break;
		case 3:
			val = h3(depth, term);
			break;
		default:
			val = h1();
			break;
	}
	return val;
}

int Board::h1(){
	int pieceDiff = countPieces(color) - countPieces(3-color);
	return pieceDiff*1000 + rand()%200;
}

int Board::h2(){
	int pieceDiff = countPieces(color) - countPieces(3-color);
	int kings = countPieces(color, true) - countPieces(3-color, true);
	//int pos = countPositions(color) - countPositions(3-color);
	int jumps = countJumps(color) - countJumps(3-color);
	return pieceDiff*500 + kings*300 + jumps*200 + rand()%30;
}

int Board::h3(int depth, bool term){
	int val = h3begin(color,depth)-h3begin(3-color, depth);
	if(term) val += (20-depth)*10;
	return val;
	
}

int Board::h3begin(int clr, int depth){
	int a, col, turn;
	int pieceWeight,kingWeight,posWeight,rowWeight,sideWeight,distWeight,turnBonus,backRowBonus,depthSub;
	int count = 0;
	int dist = 0;
	int pieceDiff = 0;
	if(end_section){
		pieceWeight = 150;
		kingWeight = 250;
		posWeight = 0;
		rowWeight = 25;
		sideWeight = 15;
		distWeight = 10;
		turnBonus = 20;
		backRowBonus = 20;
	} else {
		pieceWeight = 150;
		kingWeight = 150;
		posWeight = 10;
		rowWeight = 3;
		sideWeight = 15;
		distWeight = 0;
		turnBonus = 20;
		backRowBonus = 20;
	}
	
	for(int row=0; row<8; row++){
		a = (clr==1)?row:7-row; //rows from bottom
		for(col=0; col<4; col++){
			if(square[row][col]->color==clr){ 
				//Add Piece Bonus
				count += pieceWeight;

				//Add King Bonus
				if(square[row][col]->king) count+= kingWeight;
				
				//ADD Pos Bonus
				if(a>4) count += posWeight;
				if(a>3) count+= a*rowWeight;

				//ADD SIDES
				if(!col || col==3) count+= sideWeight;

				//Add to distances
				dist += distances(row, col, 3-clr); //Distance from this space to other player's kings
				pieceDiff++;
			}
			else if(square[row][col]->color==3-clr){
				pieceDiff--;
			}
		}
	}

	//Distances Weight. Maximize if you have more pieces than other player.
	dist*=distWeight;
	if(pieceDiff>0) count+=distWeight;
	else if(pieceDiff<0) count-=distWeight;

	//Back Row Bonus
	a = color==1?0:7; int br = 0;
	for(col=0;col<4;col++) {
		if(!square[a][col]->color) br++;
	}
	if(br<2) count += br*backRowBonus;

	//TURN Bonus
	turn = depth%2;
	if(color==clr) turn++;
	count += turn*turnBonus;

	return count*100 + rand()%20;
}

int Board::countPieces(int color, bool king){
	int count = 0;
	for(int row=0; row<8; row++){
		for(int col=0; col<4; col++){
			if(square[row][col]->color==color){ 
				if(!color || (king && !square[row][col]->king)) continue;
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
				dist += abs(row-r) + abs(col-c)/2;
			}
		}
	}	
}