#include "King.h"
#include "Globals.h"

static const double VALUE = 900;

static double valueAdditives[8][8] =
{ -3.0,-4.0,-4.0,-5.0,-5.0,-4.0,-4.0,-3.0,
  -3.0,-4.0,-4.0,-5.0,-5.0,-4.0,-4.0,-3.0,
  -3.0,-4.0,-4.0,-5.0,-5.0,-4.0,-4.0,-3.0,
  -3.0,-4.0,-4.0,-5.0,-5.0,-4.0,-4.0,-3.0, 
  -2.0,-3.0,-3.0,-4.0,-4.0,-3.0,-3.0,-2.0, 
  -1.0,-2.0,-2.0,-2.0,-2.0,-2.0,-2.0,-1.0, 
   2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 
   2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0};


King::King(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "King") {}

List<Location> kingMoves(const Location& location, int horizontalScale, int verticalScale, bool shouldCheckForCastling = false)
{
	List<Location> loc;
	
	Location newLocation;
	newLocation.row = location.row + verticalScale;
	newLocation.column = location.column + horizontalScale;
	loc.pushFront(newLocation);
	
	if (shouldCheckForCastling && (location.row == KingRowBottom || location.row == KingRowTop) && location.column == KingColumn)
	{
		Location kingCastlingLocation(location.row, location.column + horizontalScale * 2);
		loc.pushFront(kingCastlingLocation);
	}

	return loc;
}

List<List<Location>> King::possibleMoves(const Location& location) 
{
	List<List<Location>> result;
	List<Location> list = kingMoves(location, 0, 1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 1, 0, true);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 0, -1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, -1, 0, true);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 1, 1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 1, -1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, -1, 1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, -1, -1);
	result.pushFront(filteredLocations(list));
	
	

	return result;
}

double King::getValueForPosition(Location location)
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
