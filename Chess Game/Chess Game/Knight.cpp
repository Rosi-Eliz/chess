#include "Knight.h"
#include <math.h>
#define maxDeviation 2

static const double VALUE = 30;

static double valueAdditives[8][8] =
{ -5.0,-4.0,-3.0,-3.0,-3.0,-3.0,-4.0,-5.0,
  -4.0,-2.0, 0.0, 0.0, 0.0, 0.0,-2.0,-4.0,
  -3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0,-3.0,
  -3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5,-3.0,
  -3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0,-3.0,
  -3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5,-3.0,
  -4.0,-2.0, 0.0, 0.5, 0.5, 0.0,-2.0,-4.0,
  -5.0,-4.0,-2.0,-3.0,-3.0,-2.0,-4.0,-5.0, };

Knight::Knight(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "Knight") {}


List<Location> findPositions(const Location& currentLocation, int rowIndex, int colIndex)
{
	List<Location> list;
	int j{ 0 };
	for (int i{ 1 }; i <= 4; i++)
	{
		Location newLocation;
		newLocation.row = currentLocation.row + rowIndex * int(pow(-1, i));
		j += i;
		newLocation.column = currentLocation.column + colIndex * int(pow(-1, j));
		list.pushFront(newLocation);
	}
	return list;
}

List<List<Location>> Knight::possibleMoves(const Location& location)
{
	List<Location> foundPositions;
	foundPositions = findPositions(location, 2, 1);
	foundPositions += findPositions(location, 1, 2);
	List<List<Location>> resultPositions;
	
	List<Location> filteredPositions = filteredLocations(foundPositions);
	for (int i{ 0 }; i < filteredPositions.size(); i++)
	{
		List <Location> newList;
		newList.pushFront(filteredPositions[i]);
		resultPositions.pushFront(newList);
	}
	return resultPositions;
}

double Knight::getValueForPosition(Location location)
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
