#pragma once
#include "Figure.h"

class Knight : Figure {


public:
	Knight(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};