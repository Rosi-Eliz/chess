#include "Figure.h"

Figure::Figure(ChessFigureColor color, ChessFigureDirection direction, string name)
{
	this->color = color;
	this->direction = direction;
	this->name = name;
}

Figure::Figure() {}

Figure::Figure(const Figure& figure) {
	color = figure.color;
	direction = figure.direction;
	name = figure.name;
}

ChessFigureColor Figure::getColor() const
{
	return color;
}

string Figure::getColorString() const
{
	switch (color)
	{
	case ChessFigureColor::Black: 
		return "Black"; 

	case ChessFigureColor::White:
		return "White"; 
	}
	return "";
}

string Figure::getName() {
	return name;
}

ChessFigureDirection Figure::getDirection() const
{
	return direction;
}

List<Location> Figure::filteredLocations(List<Location>& unfiltered) const
{
	List<Location> resultPositions;
	for (int i{ 0 }; i < unfiltered.size(); i++)
	{
		Location currentLocation = unfiltered[i];
		bool upperRangeSatisfied = currentLocation.row < ChessBoardRows&& currentLocation.column < ChessBoardColumns;
		bool lowerRangeSatisfied = currentLocation.row >= 0 && currentLocation.column >= 0;

		if (lowerRangeSatisfied && upperRangeSatisfied)
		{
			resultPositions.pushFront(currentLocation);
		}
	}

	return resultPositions;
}

List<List<Location>> Figure::possibleMoves(const Location& location)
{
	List<List<Location>> emptyList;
	return emptyList;
}