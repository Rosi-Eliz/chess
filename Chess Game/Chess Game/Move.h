#pragma once
#include "Globals.h"
#include "Figure.h"


class Move {
	Location from; 
	Location to;
	ChessFigureColor playerColor;
	ChessFigureDirection playerDirection;
	Figure* removedFigure;
public:
	Move(Location from, Location to, ChessFigureColor playerColor, ChessFigureDirection playerDirection, Figure* removedFigure);
	
};
