#include "Rook.h"
using namespace std;


Rook::Rook(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction) {}

List<Location> moves(const Location& location, int horizontalScale, int verticalScale)
{
	List<Location> loc;
	for (int i{ 1 }; i < ChessBoardRows; i++)
	{
		Location newLocation;
		newLocation.row = location.row + verticalScale * i;
		newLocation.column = location.row + horizontalScale * i;
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