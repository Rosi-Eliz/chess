#include "Game.h"
#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "SFMLGraphicsEngine.h"

void didRemoveFigure(int row, int column);
bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn);
void didMove(int fromRow, int fromColumn, int toRow, int toColumn);
bool isPlayerActive(FigureType figureType);
List<Location> availableMovesForFigure(int row, int column);

ChessFigureColor figuresTurn = ChessFigureColor::White;
Board* boardReference;

Game::Game() {
}

Game::Game(ChessBoardLayout layout)
{
	board = Board(layout);
	boardReference = &board;

	graphicsEngine = SFMLGraphicsEngine();
	graphicsEngine.didMove = didMove;
	graphicsEngine.availableMovesForFigure = availableMovesForFigure;
	graphicsEngine.didRemoveFigure = didRemoveFigure;
	graphicsEngine.isMoveValid = isMoveValid;
	graphicsEngine.isPlayerActive = isPlayerActive;
}

void switchTurn()
{
	int currentTurn = static_cast<int>(figuresTurn);
	figuresTurn = static_cast<ChessFigureColor>((currentTurn + 1) % 2);
}

void Game::initiateGame()
{
	BoardLayout boardLayout = board.getLayout() == ChessBoardLayout::LeadingBlacks ? 
	BoardLayout::LeadingBlacks : BoardLayout::LeadingWhites;

	populateFigures();
	graphicsEngine.initiateRender(boardLayout);
}

void Game::populateFigures()
{
	List<Field*> fields = board.getFields(); 
	for (int i{ 0 }; i < fields.size(); i++)
	{
		Field* currentField = fields[i];
		if (currentField == nullptr || currentField->getFigure() == nullptr) 
		{
			continue;
		}
		Figure* figure = currentField->getFigure();

		FigureType figureType = figure->getColor() == ChessFigureColor::Black ? Black : White;
		Location currentLocation = currentField->getLocation();
		int row = currentLocation.row;
		int col = currentLocation.column;

		if (typeid(*figure) == typeid(Pawn)) 
		{
			graphicsEngine.addFigure(PawnDesignation, figureType, row, col);
		}
		else if (typeid(*figure) == typeid(King))
		{
			graphicsEngine.addFigure(KingDesignation, figureType, row, col);
		}
		else if (typeid(*figure) == typeid(Queen))
		{
			graphicsEngine.addFigure(QueenDesignation, figureType, row, col);
		}
		else if (typeid(*figure) == typeid(Rook))
		{
			graphicsEngine.addFigure(RookDesignation, figureType, row, col);
		}
		else if (typeid(*figure) == typeid(Knight))
		{
			graphicsEngine.addFigure(KnightDesignation, figureType, row, col);
		}
		else if (typeid(*figure) == typeid(Bishop))
		{
			graphicsEngine.addFigure(BishopDesignation, figureType, row, col);
		}
	}
}

void didRemoveFigure(int row, int column) {
	cout << "removed: row: " << row << ", column: " << column << endl;
}

bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) {
	return true;
}

void didMove(int fromRow, int fromColumn, int toRow, int toColumn) {
	switchTurn();
	cout << "move from - row: " << fromRow << ", column: " << fromColumn << ". To - row: ";
	cout << toRow << ", column: " << toColumn << endl;
}

bool isPlayerActive(FigureType figureType) {
	bool whiteColorMathcingCondition = figureType == White && figuresTurn == ChessFigureColor::White;
	bool blackColorMathcingCondition = figureType == Black && figuresTurn == ChessFigureColor::Black;
	return whiteColorMathcingCondition || blackColorMathcingCondition;
}

List<Location> availableMovesForFigure(int row, int column) {
	List<Location> locations;
	/*Location a1, a2;
	a1.row = 3;
	a1.column = 3;
	a2.row = 3;
	a2.column = 4;

	for (int row{ 0 }; row < ChessBoardRows; row++)
	{
		for (int col{ 0 }; col < ChessBoardColumns; col++)
		{
			Location loc(row, col);
			locations.pushFront(loc);
		}
	}
	*/
	return locations;
}
