#include "Figure.h"

Figure::Figure(ChessFigureColor color, ChessFigureDirection direction)
{
	this->color = color;
	this->direction = direction;
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