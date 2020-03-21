#pragma once
#include "Globals.h"
#include "List.h"
using namespace std;


class Figure {
protected:
	ChessFigureColor color;
	ChessFigureDirection direction;
public:
	Figure(ChessFigureColor color, ChessFigureDirection direction);
	virtual List<List<Location>> possibleMoves(const Location& location) = 0;
	
};