#include "Move.h"


Move::Move(Location from, Location to, ChessFigureColor playerColor, ChessFigureDirection playerDirection, Figure* removedFigure)
{
	this->from = from;
	this->to = to;
	this->playerColor = playerColor;
	this->playerDirection = playerDirection;
	this->removedFigure = removedFigure;
}

