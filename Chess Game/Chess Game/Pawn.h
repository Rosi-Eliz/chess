#pragma once
#include "Figure.h"
using namespace std;


class Pawn : Figure {
public:
	Pawn(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};