#include "Pawn.h"
#include <algorithm>

#define InitialRowDownDirection 6
#define InitialRowUpDirection 1
#define InitialColumn 0
#define EndColumn 7



Pawn::Pawn(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction) {}

List<List<Location>> Pawn::possibleMoves(const Location& location) 
{
	List<List<Location>> list;
	
	int directionScale = direction == ChessFigureDirection::Up ? 1 : -1;
	int verticalSteps = 1;
	bool startAndDown = location.row == InitialRowDownDirection && direction == ChessFigureDirection::Down;
	bool startAndUp = location.row == InitialRowUpDirection && direction == ChessFigureDirection::Up;
	if(startAndDown || startAndUp)
	{ 
		verticalSteps++;
	}
	if (location.column != InitialColumn ) //Left diagonal logic 
	{
		List<Location> leftDiagonal;
		Location newLocation; 
		newLocation.row = location.row + directionScale;
		newLocation.column = location.column - 1;
		leftDiagonal.pushFront(newLocation);
		list.pushFront(leftDiagonal);
	}
	if (location.column != EndColumn) //Right diagonal logic 
	{
		List<Location> rightDiagonal;
		Location newLocation;
		newLocation.row = location.row + directionScale;
		newLocation.column = location.column + 1;
		rightDiagonal.pushFront(newLocation);
		list.pushFront(rightDiagonal);
	}
	List<Location> forwardSteps;
	for (int i{ 1 }; i <= verticalSteps; i++)
	{
		Location newLocation; 
		newLocation.row = location.row + directionScale*i;
		newLocation.column = location.column;
		forwardSteps.pushFront(newLocation);
	}
	list.pushFront(forwardSteps);
	return list;
}