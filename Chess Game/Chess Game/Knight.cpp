#include "Knight.h"

#define maxDeviation 2

Knight::Knight(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction) {}


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