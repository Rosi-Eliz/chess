#include "King.h"


King::King(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "King") {}

List<Location> kingMoves(const Location& location, int horizontalScale, int verticalScale)
{
	List<Location> loc;
	
	Location newLocation;
	newLocation.row = location.row + verticalScale;
	newLocation.column = location.row + horizontalScale;
	loc.pushFront(newLocation);
	return loc;
}

List<List<Location>> King::possibleMoves(const Location& location) 
{
	List<List<Location>> result;
	List<Location> list = kingMoves(location, 0, 1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 1, 0);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, 0, -1);
	result.pushFront(filteredLocations(list));

	list = kingMoves(location, -1, 0);
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