#include "square.h"

Square::Square(int pcolor, int xloc, int yloc){
	occupied = (bool) pcolor;
	color = pcolor;
	x = xloc; y = yloc;

	king = false; //INITIALIZED
}