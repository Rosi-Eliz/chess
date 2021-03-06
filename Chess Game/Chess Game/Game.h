#pragma once
#include "Board.h"
#include "SFMLGraphicsEngine.h"
#include "AIUpdateProvider.h"
#include "AIEngine.h"
#include "GameInteraction.h"
#include "Move.h"
#include <time.h>

class Game : public GameInteraction, public GraphicsEngineProvider, public AIUpdateProvider
{
	// We are emulating an interface based structure, which is not supported in C++
	// so as to achieve a cross-platform application, therefore only GraphicsEngine members
	// should be accessed/modified from here.
	SFMLGraphicsEngine graphicsEngine;
	AIEngine artificialIntelligenceEngine;
	ChessFigureColor figuresTurn;

	List<Move*> completedMoves;

	Board *board;
	State checkState();
	time_t startTime;
	void createMoveLog(Location from, Location to);
	void didRemoveFigure(int row, int column) override;
	bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) override;
	void didMove(int fromRow, int fromColumn, int toRow, int toColumn) override;
	bool isPlayerActive(FigureType figureType) override;
	List<Location>availableMovesForFigure(int row, int column) override;
	void renderNewFigure(Figure* figure, int row, int col);
	void switchTurn();
	void restart(ChessBoardLayout layout);
	
public:
	Game();
	Game(ChessBoardLayout layout);
	Game(const Game& game);
	Game& operator=(const Game& game);
	~Game();
	void initiateGame();
	void populateFigures();
	void removeFigureAt(int row, int col) override;
	void addFigureAt(int row, int col) override;
	void didRestartGame() override;
	void didChooseBestMove(int fromRow, int fromCol, int toRow, int toCol) override;
	void move(int fromRow, int fromCol, int toRow, int toCol, bool shouldNotifyReceiver = true) override;
	void revertLastMove() override;
};