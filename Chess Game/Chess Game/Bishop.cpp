#include "Bishop.h"

static const double VALUE = 30;

static double valueAdditives[8][8] =
{ -2.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-2.0,
  -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,-1.0,
  -1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0,-1.0,
  -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,-1.0,
  -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,-1.0,
  -1.0,-2.0,-2.0,-2.0,-2.0,-2.0,-2.0,-1.0,
  -1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5,-1.0,
  -2.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-2.0 };

Bishop::Bishop(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "Bishop") {}


List<Location> diagonal(const Location& location, int horizontalScale, int verticalScale)
{
	List<Location> locations;
	for (int i{ 1 }; i < ChessBoardRows; i++)
	{
		Location newLocation;
		newLocation.row = location.row + horizontalScale * i;
		newLocation.column = location.column + verticalScale * i;
		locations.pushFront(newLocation);
	}
	return locations;
}


List<List<Location>> Bishop::possibleMoves(const Location& location)
{
	List<List<Location>> resultMoves;

	List<Location> moves = diagonal(location, 1, 1);
	List<Location> filtered = filteredLocations(moves);
	resultMoves.pushFront(filtered);

	moves = diagonal(location, -1, 1);
	filtered = filteredLocations(moves);
	resultMoves.pushFront(filtered);

	moves = diagonal(location, -1, -1);
	filtered = filteredLocations(moves);
	resultMoves.pushFront(filtered);

	moves = diagonal(location, 1, -1);
	filtered = filteredLocations(moves);
	resultMoves.pushFront(filtered);

	return resultMoves;
}

double Bishop::getValueForPosition(Location location)
{
	double value = VALUE;
	if (direction == ChessFigureDirection::Up)
	{
		value += valueAdditives[location.row][location.column];
	}
	else
	{
		double** newAdditives = reversedMatrix(valueAdditives);
		value += newAdditives[location.row][location.column];
		for (int i{ 0 }; i < 8; i++)
		{
			delete[] newAdditives[i];
		}
		delete[] newAdditives;
	}
	return value;
}
