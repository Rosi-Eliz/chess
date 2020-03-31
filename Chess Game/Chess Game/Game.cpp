#include "Game.h"
#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "Figure.h"
#include "SFMLGraphicsEngine.h" 
#include <functional>

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

	/*
	List<Figure*> blackFigures = board.remainingFigures(ChessFigureColor::Black);

	auto print = [](Figure* figure) {
		cout << figure->getName() << " " << figure->getColorString() << endl;
	};
	blackFigures.forEach(print);
	*/

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
	List<Location> moves = availableMovesForFigure(fromRow, fromColumn);
	return moves.contains([toRow, toColumn](Location location) {
		return location.column == toColumn && location.row == toRow;
	});
}

void didMove(int fromRow, int fromColumn, int toRow, int toColumn) {
	switchTurn();
	Location oldLocation = Location(fromRow, fromColumn);
	Location newLocation = Location(toRow, toColumn);
	boardReference->updateMove(oldLocation, newLocation);
	cout << "move from - row: " << fromRow << ", column: " << fromColumn << ". To - row: ";
	cout << toRow << ", column: " << toColumn << endl;
}

bool isPlayerActive(FigureType figureType) {
	bool whiteColorMathcingCondition = figureType == White && figuresTurn == ChessFigureColor::White;
	bool blackColorMathcingCondition = figureType == Black && figuresTurn == ChessFigureColor::Black;
	return whiteColorMathcingCondition || blackColorMathcingCondition;
}

List<List<Location>> pawnDiagonalPossibleMoves(Figure* pawn, Location& location, List<List<Location>>&allMoves)
{
	List<List<Location>> filteredLocations;
	ChessFigureColor currentColor = pawn->getColor();
	int direction = static_cast<int>(pawn->getDirection());
	int diagonalRow = pow(-1, direction);
	Location leftD = Location(diagonalRow + location.row, location.column - 1);
	Location rightD = Location(diagonalRow + location.row, location.column + 1);
	Field* f1 = boardReference->getFieldAt(leftD);
	Field* f2 = boardReference->getFieldAt(rightD);
	
	bool leftDiagonalRowOpponent = f1 != nullptr && f1->getFigure() != nullptr && f1->getFigure()->getColor() != currentColor;
	bool rightDiagonalRowOpponent = f2 != nullptr && f2->getFigure() != nullptr && f2->getFigure()->getColor() != currentColor;
	auto pawnFilter = [leftDiagonalRowOpponent, rightDiagonalRowOpponent, leftD, rightD](List<Location> locations)
	{
		for (size_t i{ 0 }; i < locations.size(); i++) {
			Location currentLocation = locations[i];
			if (currentLocation.column == leftD.column)
			{
				return leftDiagonalRowOpponent;
			}
			else if (currentLocation.column == rightD.column)
			{
				return rightDiagonalRowOpponent;
			}
			
		}
		return true;
	};
	
	return allMoves.filter(pawnFilter);
}

/*

l = {{1},{2,3}}
filter forEach{ el
if(el>2) return false
}
l.filter(t) = 

pawn: r:1, c:1
{{(2,0)},
{(2,1),(3,1)},
{(2,2)}}

{{(2,1),(3,1)},
{2,2}}
{(2,1),(3,1),(2,2)}


*/

List<Location> availableMovesForFigure(int row, int column) {
	List<List<Location>> locations;

	Location givenLocation = Location(row, column);
	Figure* figure = boardReference->figureAt(givenLocation);
	List<List<Location>> moves = figure->possibleMoves(givenLocation);
	if (typeid(*figure) == typeid(Pawn)) 
	{
		locations = pawnDiagonalPossibleMoves(figure,givenLocation, moves);
	} 
	else
	{
		locations = moves;
	}

	List<Location> result;
	locations.forEach([&result, &figure, &givenLocation](List<Location> list) {
		for (int j{ 0 }; j < list.size(); j++)
		{
			Location location = list[j];
			Field* f = boardReference->getFieldAt(location);
			if (f->getFigure() == nullptr)
			{
				result.pushFront(location);
			}
			else if (f->getFigure()->getColor() != figure->getColor())
			{
				//forbid forward pawn moves in case of opponent figure in front 
				if (typeid(Pawn) != typeid(*figure) || location.column != givenLocation.column)
				{
					result.pushFront(location);
				}
				break;
			}
			else
			{
				break;
			}
		}
	});

	return result;
}
