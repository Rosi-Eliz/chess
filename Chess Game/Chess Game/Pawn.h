#pragma once
#include "Figure.h"
#include "Globals.h"
using namespace std;


class Pawn : public Figure {
public:
	Pawn(ChessFigureColor color, ChessFigureDirection direction);
	List<List<Location>> possibleMoves(const Location& location) override;
};