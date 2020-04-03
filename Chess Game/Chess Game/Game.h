#pragma once
#include "Board.h"
#include "SFMLGraphicsEngine.h"
#include "GameInteraction.h"


class Game : public GameInteraction, public GraphicsEngineProvider 
{
	// We are emulating an interface based structure, which is not supported in C++
	// so as to achieve a cross-platform application, therefore only GraphicsEngine members
	// should be accessed/modified from here.
	SFMLGraphicsEngine graphicsEngine;

	ChessFigureColor figuresTurn;

	bool bottomRightCaslingIsPossible = true;
	bool bottomLeftCaslingIsPossible = true;
	bool topRightCaslingIsPossible = true;
	bool topLeftCaslingIsPossible = true;

	Board board;
	void didRemoveFigure(int row, int column) override;
	bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) override;
	void didMove(int fromRow, int fromColumn, int toRow, int toColumn) override;
	bool isPlayerActive(FigureType figureType) override;
	List<Location> availableMovesForFigure(int row, int column) override;
	List<List<Location>> pawnDiagonalPossibleMoves(Figure* pawn, Location& location, List<List<Location>>& allMoves);
	void renderNewFigure(Figure* figure, int row, int col);
	void switchTurn();
	void castlingPossible(int fromRow, int fromColumn);
	List<List<Location>> filteredKingCastlingMoves(int row, int column, List<List<Location>>& allMoves);
	
public:
	Game();
	Game(ChessBoardLayout layout);
	void initiateGame();
	void populateFigures();
	void removeFigureAt(int row, int col) override;
	void addFigureAt(int row, int col) override;
};