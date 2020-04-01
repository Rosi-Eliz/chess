#pragma once
#include "Board.h"
#include "SFMLGraphicsEngine.h"
#include "GameInteraction.h"

class Game : public GameInteraction 
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

	void renderNewFigure(Figure* figure, int row, int col);
public:
	Game();
	Game(ChessBoardLayout layout);
	void initiateGame();
	void populateFigures();
	void removeFigureAt(int row, int col) override;
	void addFigureAt(int row, int col) override;
};