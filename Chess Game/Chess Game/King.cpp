#include "King.h"
#include "Globals.h"

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