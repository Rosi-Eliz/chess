#pragma once
#include "Globals.h"
#include "Figure.h"
#include <time.h>

class Move {
	Location from; 
	Location to;
	ChessFigureColor playerColor;
	ChessFigureDirection playerDirection;
	Figure* removedFigure;
	time_t timestamp;
public:
	Move(Location from, Location to, ChessFigureColor playerColor, ChessFigureDirection playerDirection, Figure* removedFigure);
	string getDurationSince(time_t startTime) const;
	time_t getTime() const;
};
