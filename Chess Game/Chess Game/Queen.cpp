#include "Queen.h"

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