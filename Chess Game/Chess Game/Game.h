#pragma once
#include "Board.h"
#include "SFMLGraphicsEngine.h"

class Game
{
	// We are emulating an interface based structure, which is not supported in C++
	// so as to achieve a cross-platform application, therefore only GraphicsEngine members
	// should be accessed/modified from here.
	SFMLGraphicsEngine graphicsEngine;

	Board board;
	friend void didRemoveFigure(int row, int column);
	friend bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn);
	friend void didMove(int fromRow, int fromColumn, int toRow, int toColumn);
	friend bool isPlayerActive(FigureType figureType); 
	friend List<Location> availableMovesForFigure(int row, int column);
public:
	Game();
	Game(ChessBoardLayout layout);
	void initiateGame();
	void populateFigures();
};