#pragma once
#include "Figure.h"

class Bishop : public Figure{
public:
	Bishop(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location);
};