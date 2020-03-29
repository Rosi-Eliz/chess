#pragma once
#include "Figure.h"

class Rook : public Figure{

public:
	Rook(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};

