#pragma once
#include "Figure.h"

class Knight : public Figure {


public:
	Knight(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
	double getValueForPosition(Location location) override;
};