#pragma once
#include "Figure.h"


class Queen : public Figure {
public:
	Queen(ChessFigureColor color, ChessFigureDirection direction);
    List<List<Location>> possibleMoves(const Location& location) override;
	double getValueForPosition(Location location);
};