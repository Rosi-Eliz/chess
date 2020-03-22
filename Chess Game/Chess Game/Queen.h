#pragma once
#include "Figure.h"


class Queen : Figure {
public:
	Queen(ChessFigureColor color, ChessFigureDirection direction);
    List<List<Location>> possibleMoves(const Location& location) override;
};