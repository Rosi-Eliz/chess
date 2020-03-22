#pragma once
#include "Figure.h"

class Bishop : Figure{
public:
	Bishop(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location);
};