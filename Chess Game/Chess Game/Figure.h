#pragma once
#include "Globals.h"
#include "List.h"
using namespace std;


class Figure {
protected:
	ChessFigureColor color;
	string name;
	ChessFigureDirection direction;
	List<Location> filteredLocations(List<Location>& unfiltered) const;
	double** reversedMatrix(double matrix[8][8]);

public:
	Figure();
	Figure(ChessFigureColor color, ChessFigureDirection direction, string name);
	Figure(const Figure& figure);
	virtual List<List<Location>> possibleMoves(const Location& location);
	virtual string getName();
	ChessFigureColor getColor() const;
	string getColorString() const;
	ChessFigureDirection getDirection() const;
	virtual double getValueForPosition(Location location);
};