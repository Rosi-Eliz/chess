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

ChessFigureColor returnOpponentColor(ChessFigureColor color)
{
	ChessFigureColor oppositePlayerColor = color == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;
	return oppositePlayerColor;
}

Game::Game() {
}


Game::Game(ChessBoardLayout layout)
{
	graphicsEngine = SFMLGraphicsEngine(this);
	board = Board(layout, this);
	figuresTurn = ChessFigureColor::White;
}

State Game::checkState() 
{
	List<Figure*> currentPlayerPossibleMoves = board.remainingFigures(figuresTurn);
	for (int i{ 0 }; i < currentPlayerPossibleMoves.size(); i++)
	{
		Figure* currentFigure = currentPlayerPossibleMoves[i];
		Field* field = board.getField(currentFigure);
		Location location = field->getLocation();
		if (availableMovesForFigure(location.row, location.column).size() > 0)
		{
			return State::InProgress;
		}
	}
	ChessFigureColor opponentColor = returnOpponentColor(figuresTurn);
	bool conflict = fieldIsInConflict(opponentColor);
	if (conflict)
	{
		switch (opponentColor)
		{
		case ChessFigureColor::Black: 
			return State::BlackWins; 
		case ChessFigureColor::White:
			return State::WhiteWins;
		}
	}
	return State::Draw;

}

void Game::switchTurn()
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

		Location currentLocation = currentField->getLocation();
		int row = currentLocation.row;
		int col = currentLocation.column;
		renderNewFigure(figure, row, col);
	}
}

void Game::renderNewFigure(Figure* figure, int row, int col) {
	FigureType figureType = figure->getColor() == ChessFigureColor::Black ? Black : White;
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

void Game::didRemoveFigure(int row, int column) {


	cout << "removed: row: " << row << ", column: " << column << endl;
}

bool Game::isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) {
	List<Location> moves = availableMovesForFigure(fromRow, fromColumn);
	return moves.contains([toRow, toColumn](Location location) {
		return location.column == toColumn && location.row == toRow;
	});
}

void Game::castlingPossible(int fromRow, int fromColumn)
{
	if (fromRow == RookBottomLeftRow && fromColumn == RookBottomLeftCol)
	{
		bottomLeftCaslingIsPossible = false;
	}
	else if (fromRow == RookBottomRightRow && fromColumn == RookBottomRightCol)
	{
		bottomRightCaslingIsPossible = false;
	}
	else if (fromRow == RookTopLeftRow && fromColumn == RookTopLeftCol)
	{
		topLeftCaslingIsPossible = false;
	}
	else if (fromRow == RookTopRightRow && fromColumn == RookTopRightCol)
	{
		topRightCaslingIsPossible = false;
	}
	else if (fromRow == KingRowBottom && fromColumn == KingColumn)
	{
		bottomLeftCaslingIsPossible = false;
		bottomRightCaslingIsPossible = false;
	}
	else if (fromRow == KingRowTop && fromColumn == KingColumn)
	{
		topRightCaslingIsPossible = false;
		topLeftCaslingIsPossible = false;
	}
}

void Game::moveRookInCastling(int fromRow, int fromColumn, int toRow, int toColumn) {
	Location oldLocation = Location(fromRow, fromColumn);
	Location newLocation = Location(toRow, toColumn);
	Field* field = board.getFieldAt(oldLocation);
	if (field == nullptr)
		throw runtime_error("no such field");

	Figure* figure = field->getFigure();

	if (typeid(*figure) != typeid(King))
		return;

	int directionOfMovement = toColumn - fromColumn;
	if (directionOfMovement == 2)
	{
		Location initialRookLocation = Location(toRow, RookBottomRightCol);
		Location finalRookLocation = Location(toRow, RookBottomRightCol - ShortCastlingDistance);
		board.updateMove(initialRookLocation, finalRookLocation);
		graphicsEngine.move(toRow, RookBottomRightCol, toRow, RookBottomRightCol - ShortCastlingDistance);
	}
	else if (directionOfMovement == -2)
	{
		Location initialRookLocation = Location(toRow, RookBottomLeftCol);
		Location finalRookLocation = Location(toRow, RookBottomLeftCol + LongCastlingDistance);
		board.updateMove(initialRookLocation, finalRookLocation);
		graphicsEngine.move(toRow, RookBottomLeftCol, toRow, RookBottomLeftCol + LongCastlingDistance);
	}
}

void Game::didMove(int fromRow, int fromColumn, int toRow, int toColumn) {
	switchTurn();
	Location oldLocation = Location(fromRow, fromColumn);
	Location newLocation = Location(toRow, toColumn);

	//if(typeid(movedFigure) == typeid(Rook))
	moveRookInCastling(fromRow, fromColumn, toRow, toColumn);
	castlingPossible(fromRow, fromColumn);
	board.updateMove(oldLocation, newLocation);

	cout << "state: " << static_cast<int>(checkState()) << endl;
	cout << "move from - row: " << fromRow << ", column: " << fromColumn << ". To - row: ";
	cout << toRow << ", column: " << toColumn << endl;
}

bool Game::isPlayerActive(FigureType figureType) {
	bool whiteColorMathcingCondition = figureType == White && figuresTurn == ChessFigureColor::White;
	bool blackColorMathcingCondition = figureType == Black && figuresTurn == ChessFigureColor::Black;
	return whiteColorMathcingCondition || blackColorMathcingCondition;
}

List<List<Location>> Game::pawnDiagonalPossibleMoves(Figure* pawn, Location& location, List<List<Location>>&allMoves)
{
	List<List<Location>> filteredLocations;
	ChessFigureColor currentColor = pawn->getColor();
	int direction = static_cast<int>(pawn->getDirection());
	int diagonalRow = pow(-1, direction);
	Location leftD = Location(diagonalRow + location.row, location.column - 1);
	Location rightD = Location(diagonalRow + location.row, location.column + 1);
	Field* f1 = board.getFieldAt(leftD);
	Field* f2 = board.getFieldAt(rightD);
	
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

bool Game::movementIsPossibleInCastling(int fromRow, int fromCol, int toRow, int toCol)
{
	Location oldLocation(fromRow, fromCol);
	Field* field = board.getFieldAt(oldLocation);
	if (field == nullptr)
	{
		return false;
	}
	Figure* movedFigure = field->getFigure();
	if (movedFigure == nullptr)
	{
		return false;
	}

	Location newLocation(toRow, toCol);
	Field* newField = board.getFieldAt(newLocation);
	if (newField == nullptr)
	{
		return false;
	}
	Figure* otherFigure = newField->getFigure();

	if (field == nullptr || movedFigure || newField == nullptr)

	newField->setFigure(movedFigure);
	field->setFigure(nullptr);

	bool isInConflict = fieldIsInConflict(returnOpponentColor(movedFigure->getColor()));
	newField->setFigure(otherFigure);
	field->setFigure(movedFigure);
	return !isInConflict;
}

List<List<Location>> Game::filteredKingCastlingMoves(int row, int column, List<List<Location>>& allMoves)
{
	List<List<Location>> result;

	allMoves.forEach([&](List<Location> list) {
		List<Location> subLocations = list.filter([&](Location location) {
			if (location.column - column == -2 && row == SupplementaryRowUpDirection)
				return bottomLeftCaslingIsPossible;
			if (location.column - column == 2 && row == SupplementaryRowUpDirection)
				return bottomRightCaslingIsPossible;
			if (location.column - column == 2 && row == SupplementaryRowDownDirection)
				return topRightCaslingIsPossible;
			if (location.column - column == -2 && row == SupplementaryRowDownDirection)
				return topLeftCaslingIsPossible;
			return true;
		});
		result.pushFront(subLocations);
	});
	return result;
}

List<Location> Game::filteredMoves(int row, int column)
{
	List<List<Location>> locations;

	Location givenLocation = Location(row, column);
	Figure* figure = board.figureAt(givenLocation);
	List<List<Location>> moves = figure->possibleMoves(givenLocation);
	if (typeid(*figure) == typeid(Pawn))
	{
		locations = pawnDiagonalPossibleMoves(figure, givenLocation, moves);
	}
	else if (typeid(*figure) == typeid(King))
	{
		locations = filteredKingCastlingMoves(row, column, moves);
	}
	else
	{
		locations = moves;
	}

	List<Location> result;
	locations.forEach([&](List<Location> list) {
		for (int j{ 0 }; j < list.size(); j++)
		{
			Location location = list[j];
			Field* f = board.getFieldAt(location);
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


bool Game::fieldIsInConflict(ChessFigureColor opponentColor)
{
	ChessFigureColor currentPlayerColor = opponentColor == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;
	Figure* king = board.getKing(currentPlayerColor);
	Field* kingField = board.getField(king);
	List<Figure*> opponentFigures = board.remainingFigures(opponentColor);
	for (int i{ 0 }; i < opponentFigures.size(); i++)
	{

		Figure* currentFigure = opponentFigures[i];
		Field* currentFigureField = board.getField(currentFigure);
		List<Location> moves = filteredMoves(currentFigureField->getLocation().row,
											 currentFigureField->getLocation().column);
		
		for (int j{ 0 }; j < moves.size(); j++)
		{
			Location currentLocation = moves[j];
			if (kingField->getLocation().row == currentLocation.row &&
				kingField->getLocation().column == currentLocation.column)
				return true;
		}
	}
	return false;
}

List<Location> Game::filteredConflictMoves(List<Location> availableMoves, int row, int column)
{
	Location givenLocation = Location(row, column);
	Field* startingField = board.getFieldAt(givenLocation);
	Figure* figure = board.figureAt(givenLocation);
	ChessFigureColor opponentColor = figure->getColor() == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;
	
	if (startingField != nullptr) {
		startingField->setFigure(nullptr);
	}
	List<Location> allowedMoves = availableMoves.filter([&](Location currentLocation) {

		Field* newField = board.getFieldAt(currentLocation);
		Figure* oldFigure = newField->getFigure();
		newField->setFigure(figure);
		bool isConflict = fieldIsInConflict(opponentColor);
		newField->setFigure(oldFigure);
		return !isConflict;
		
	});
	startingField->setFigure(figure);
	return allowedMoves;
}

List<Location> Game::removeKingGapLocation(List<Location> location, int row, int column) {
	if ((row != KingRowBottom || column != KingColumn) && (row != KingRowTop || column != KingColumn))
		return location;

	Figure* figure = board.figureAt(Location(row, column));
	
	if (figure == nullptr || typeid(*figure) != typeid(King))
	{
		return location;
	}

	bool leftFieldIsValid = location.contains([&](Location loc) {
		return loc.column == column - 1 && loc.row == row;
	});
	bool rightFieldIsValid = location.contains([&](Location loc) {
		return loc.column == column + 1 && loc.row == row;
	});

	bool conflict = fieldIsInConflict(returnOpponentColor(figure->getColor()));

	if(!leftFieldIsValid || conflict)
	{ 
		location = location.filter([&](Location loc) {
			return loc.column != column - 2;
		});
	}
	if (!rightFieldIsValid || conflict)
	{
		location = location.filter([&](Location loc) {
			return loc.column != column + 2;
		});
	}
	return location;
}

List<Location> Game::availableMovesForFigure(int row, int column) {
	List<Location> initialFiltering = filteredMoves(row, column);
	List<Location> secondaryFilter = filteredConflictMoves(initialFiltering, row, column);
	return removeKingGapLocation(secondaryFilter, row, column);
}

void Game::removeFigureAt(int row, int col)
{
	graphicsEngine.removeFigure(row, col);
}
void Game::addFigureAt(int row, int col)
{
	Location location(row, col);
	Figure* figure = board.figureAt(location);
	renderNewFigure(figure, row, col);
}
