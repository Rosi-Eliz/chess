#pragma once
#include "Figure.h"

class King : Figure{
public:

	King(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};