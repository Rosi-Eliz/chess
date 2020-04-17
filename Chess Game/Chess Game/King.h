#pragma once
#include "Figure.h"

class King : public Figure{
public:

	King(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
	double getValueForPosition(Location location) override;
};