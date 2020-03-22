#pragma once
#include "Globals.h"
#include "List.h"
using namespace std;


class Figure {
protected:
	ChessFigureColor color;
	ChessFigureDirection direction;
	List<Location> filteredLocations(List<Location>& unfiltered) const;
public:
	Figure(ChessFigureColor color, ChessFigureDirection direction);
	virtual List<List<Location>> possibleMoves(const Location& location) = 0;
	
	
};