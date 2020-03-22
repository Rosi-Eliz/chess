#include "Bishop.h"


Bishop::Bishop(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction) {}


List<Location> diagonal(const Location& location, int horizontalScale, int verticalScale)
{
	List<Location> locations;
	for (int i{ 1 }; i < ChessBoardRows; i++)
	{
		Location newLocation;
		newLocation.row = location.row + horizontalScale * i;
		newLocation.column = location.row + verticalScale * i;
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