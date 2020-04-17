#include "Pawn.h"
#include <algorithm>

static const double VALUE = 10;

static double valueAdditives[8][8] =
{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
  1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0,
  0.5, 0.5, 1.0, 2.7, 2.7, 1.0, 0.5, 0.5,
  0.0, 0.0, 0.0, 2.5, 2.5, 0.0, 0.0, 0.0,
  0.5 - 0.5,-1.0, 0.0, 0.0,-1.0,-0.5, 0.5,
  0.5, 1.0, 1.0,-2.5,-2.5, 1.0, 1.0, 0.5,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };


Pawn::Pawn(ChessFigureColor color, ChessFigureDirection direction) : Figure(color, direction, "Pawn") {}

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

double Pawn::getValueForPosition(Location location)
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