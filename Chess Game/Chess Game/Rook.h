#pragma once
#include "Figure.h"

class Rook : Figure{

public:
	Rook(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};

