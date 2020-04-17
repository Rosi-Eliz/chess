#include "Rook.h"
using namespace std;

static const double VALUE = 50;

static double valueAdditives[8][8] =
{ 
  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
  0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5,
 -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5,
 -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5,
 -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5, 
 -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5,
  0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0 };

Rook::Rook(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "Rook") {}

List<Location> moves(const Location& location, int horizontalScale, int verticalScale)
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

List<List<Location>> Rook::possibleMoves(const Location& location)
{
	List<List<Location>> result;
	List<Location> list = moves(location, 0, 1);
	result.pushFront(filteredLocations(list));

	list = moves(location, 1, 0);
	result.pushFront(filteredLocations(list));

	list = moves(location, 0, -1);
	result.pushFront(filteredLocations(list));

	list = moves(location, -1, 0);
	result.pushFront(filteredLocations(list));

	return result;
}

double Rook::getValueForPosition(Location location)
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
