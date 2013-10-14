#ifndef SQUARE_H
#define	SQUARE_H

class Square{
	public:
		int x,y;
		bool occupied;
		int color; //1 = RED, 2 = BLACK
		bool king;
		Square(int pcolor, int xloc, int yloc, bool is_king = 0);
};



#endif	/* SQUARE_H */