#include "square.h"

Square::Square(int pcolor, int xloc, int yloc, bool is_king = 0){
	occupied = (bool) pcolor;
	color = pcolor;
	x = xloc; y = yloc;
	king = is_king;
}