#include "Queen.h"

static const double VALUE = 90;

static double valueAdditives[8][8] =
{ -2.0,-1.0,-1.0,-0.5,-0.5,-1.0,-1.0,-2.0,
  -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,-1.0,
  -1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0,-1.0,
  -0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0,-0.5,
   0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0,-0.5,
  -1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0,-1.0,
  -1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0,-1.0,
  -2.0,-1.0,-1.0,-0.5,-0.5,-1.0,-1.0,-2.0 };

Queen::Queen(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "Queen") {}

List<Location> queenMoves(const Location& location, int horizontalScale, int verticalScale)
{
	List<Location> loc;
	for (int i{ 1 }; i < ChessBoardRows; i++)
	{
		Location newLocation;
		newLocation.row = location.row + verticalScale * i;
		newLocation.column = location.column + horizontalScale * i;
		loc.pushFront(newLocation);
	}
	return loc;
}

List<List<Location>> Queen::possibleMoves(const Location& location)
{
	List<List<Location>> result;
	List<Location> list = queenMoves(location, 0, 1);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, 1, 0);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, 0, -1);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, -1, 0);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, 1, 1);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, 1, -1);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, -1, 1);
	result.pushFront(filteredLocations(list));

	list = queenMoves(location, -1, -1);
	result.pushFront(filteredLocations(list));

	return result;
}

double Queen::getValueForPosition(Location location)
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
