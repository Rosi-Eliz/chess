#include "Board.h"
#include "Figure.h"
#include "Rook.h"
#include "Field.h"
#include "Bishop.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"
#include <functional>
#include <vector>
#include <unordered_map>


Board::Board(ChessBoardLayout layout, GameInteraction* gameInteraction) : layout(layout), gameInteraction(gameInteraction)
{
	initialiseFields();
	initialiseFigures();
}

Board::Board() {
}

Board& Board::operator=(const Board& board)
{
	if (this == &board)
		return *this;

	for (int i{ 0 }; i < figures.size(); i++)
	{
		delete figures[i];
	}
	figures = List<Figure*>();
	for (int i{ 0 }; i < fields.size(); i++)
	{
		delete fields[i];
	}
	fields = List<Field*>();

	*this = Board(board);
	return *this;
}

Board::Board(const Board& board)
{
	if (this == &board)
		return;

	for (int i{ 0 }; i < board.fields.size(); i++)
	{
		Field* copyField = board.fields[i];
		Field* newField = new Field(*copyField);

		if (copyField->getFigure() != nullptr)
		{
			Figure* figure = copyField->getFigure();
			Figure* newFigure = nullptr;
			if (typeid(*figure) == typeid(Pawn))
			{
				newFigure = new Pawn(figure->getColor(), figure->getDirection());
			}
			else if (typeid(*figure) == typeid(King))
			{
				newFigure = new King(figure->getColor(), figure->getDirection());
			}
			else if (typeid(*figure) == typeid(Queen))
			{
				newFigure = new Queen(figure->getColor(), figure->getDirection());
			}
			else if (typeid(*figure) == typeid(Knight))
			{
				newFigure = new Knight(figure->getColor(), figure->getDirection());
			}
			else if (typeid(*figure) == typeid(Bishop))
			{
				newFigure = new Bishop(figure->getColor(), figure->getDirection());
			}
			else if (typeid(*figure) == typeid(Rook))
			{
				newFigure = new Rook(figure->getColor(), figure->getDirection());
			}
			newField->setFigure(newFigure);
			figures.pushFront(newFigure);
		}

		fields.pushFront(newField);
	}

	bottomRightCaslingIsPossible = board.bottomRightCaslingIsPossible;
	bottomLeftCaslingIsPossible = board.bottomLeftCaslingIsPossible;
	topRightCaslingIsPossible = board.topRightCaslingIsPossible;
	topLeftCaslingIsPossible = board.topLeftCaslingIsPossible;

	layout = board.layout;
	gameInteraction = board.gameInteraction;
}

Board::~Board() {
	for (int i{ 0 }; i < figures.size(); i++)
	{
		delete figures[i];
	}
	figures = List<Figure*>();
	for (int i{ 0 }; i < fields.size(); i++)
	{
		delete fields[i];
	}
	fields = List<Field*>();
}

void Board::initialiseFields()
{
	for (int row{ 0 }; row < ChessBoardRows; row++)
	{
		for (int col{ 0 }; col < ChessBoardColumns; col++)
		{
			Location currentLocation(row, col);
			Field* field = new Field(currentLocation);
			fields.pushFront(field);
			int key = getKeyForLocation(row, col);
			fieldsMap[key] = field;
		}
	}

}

ChessBoardLayout Board::getLayout() const
{
	return layout;
}

List<Field*> Board::getFields() const
{
	return fields;
}

void Board::rookFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	Rook* rook = new Rook(color, direction);
	Location location(row, col);
	figures.pushFront(rook);
	assignFigureToField(location, rook);
}

void Board::bishopFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	Bishop* bishop = new Bishop(color, direction);
	Location location(row, col);
	figures.pushFront(bishop);
	assignFigureToField(location, bishop);
}

void Board::knightFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	Knight *knight = new Knight(color, direction);
	Location location(row, col);
	figures.pushFront(knight);
	assignFigureToField(location, knight);
}

void Board::queenFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	Queen *queen = new Queen(color, direction);
	Location location(row, col);
	figures.pushFront(queen);
	assignFigureToField(location, queen);
}

void Board::kingFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	King* king = new King(color, direction);
	Location location(row, col);
	figures.pushFront(king);
	assignFigureToField(location, king);
}

void Board::pawnFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction)
{
	Pawn* pawn = new Pawn(color, direction);
	Location location(row, col);
	figures.pushFront(pawn);
	assignFigureToField(location, pawn);
}


void Board::initialiseFigures()
{
	ChessFigureColor topFiguresColor = layout == ChessBoardLayout::LeadingWhites ? ChessFigureColor::Black : ChessFigureColor::White;
	ChessFigureColor bottomFiguresColor = layout == ChessBoardLayout::LeadingWhites ? ChessFigureColor::White : ChessFigureColor::Black;
	
	//Intialize special figures row
	
	rookFactory(SupplementaryRowDownDirection, 0, topFiguresColor, ChessFigureDirection::Down);
	knightFactory(SupplementaryRowDownDirection, 1, topFiguresColor, ChessFigureDirection::Down);
	bishopFactory(SupplementaryRowDownDirection, 2, topFiguresColor, ChessFigureDirection::Down);
	queenFactory(SupplementaryRowDownDirection, 3, topFiguresColor, ChessFigureDirection::Down);
	kingFactory(SupplementaryRowDownDirection, 4, topFiguresColor, ChessFigureDirection::Down);
	bishopFactory(SupplementaryRowDownDirection, 5, topFiguresColor, ChessFigureDirection::Down);
	knightFactory(SupplementaryRowDownDirection, 6, topFiguresColor, ChessFigureDirection::Down);
	rookFactory(SupplementaryRowDownDirection, 7, topFiguresColor, ChessFigureDirection::Down);
	//Initialize top pawns row 
	for (int i{ 0 }; i < ChessBoardColumns; i++)
	{
		pawnFactory(InitialRowDownDirection, i, topFiguresColor, ChessFigureDirection::Down);
	}

	//Intialize special figures row

	rookFactory(SupplementaryRowUpDirection, 0, bottomFiguresColor, ChessFigureDirection::Up);
	knightFactory(SupplementaryRowUpDirection, 1, bottomFiguresColor, ChessFigureDirection::Up);
	bishopFactory(SupplementaryRowUpDirection, 2, bottomFiguresColor, ChessFigureDirection::Up);
	queenFactory(SupplementaryRowUpDirection, 3, bottomFiguresColor, ChessFigureDirection::Up);
	kingFactory(SupplementaryRowUpDirection, 4, bottomFiguresColor, ChessFigureDirection::Up);
	bishopFactory(SupplementaryRowUpDirection, 5, bottomFiguresColor, ChessFigureDirection::Up);
	knightFactory(SupplementaryRowUpDirection, 6, bottomFiguresColor, ChessFigureDirection::Up);
	rookFactory(SupplementaryRowUpDirection, 7, bottomFiguresColor, ChessFigureDirection::Up);
	//Initialize bottom pawns row 
	for (int i{ 0 }; i < ChessBoardColumns; i++)
	{
		pawnFactory(InitialRowUpDirection, i, bottomFiguresColor, ChessFigureDirection::Up);
	}
}

Figure* Board::figureAt(const Location& location) 
{
	int key = getKeyForLocation(location.row, location.column);
	Field* field = fieldsMap[key];
		if (field == nullptr)
			return nullptr;

		return field->getFigure();
}

void Board::assignFigureToField(Location& location, Figure* figure)
{
	Field* field = getFieldAt(location);
	if (field != nullptr)
	{
		field->setFigure(figure);
	}
}

Field* Board::getFieldAt(const Location& location)
{
	int key = getKeyForLocation(location.row, location.column);
	return fieldsMap[key];
}

void Board::updateMove(const Location& oldLocation, const Location& newLocation, bool didPerformCastling)
{

	lastMoveWasCastling = didPerformCastling;
	MoveDescriptor moveDescriptor = MoveDescriptor(oldLocation, newLocation);
	if (didPerformCastling)
	{
		lastMoveDescriptor.addMoveExistingRecord(moveDescriptor);
	}
	else
	{
		lastMoveDescriptor.addMoveNewRecord(moveDescriptor);
	}

	moveRookInCastling(oldLocation.row, oldLocation.column, newLocation.row, newLocation.column);
	castlingPossible(oldLocation.row, oldLocation.column);

	MoveDescriptor* lastMove = new MoveDescriptor(oldLocation, newLocation);

	Field* oldField = getFieldAt(oldLocation);
	Field* newField = getFieldAt(newLocation);
	if (oldField == nullptr || newField == nullptr)
	{
		return;
	}
	Figure* movedFigure = oldField->getFigure();

	if (typeid(*movedFigure) == typeid(Pawn) && 
		(newLocation.row == SupplementaryRowDownDirection ||
			newLocation.row == SupplementaryRowUpDirection))
	{
		
		if (gameInteraction != nullptr)
		{
			gameInteraction->removeFigureAt(newLocation.row, newLocation.column);
		}
		queenFactory(newLocation.row, newLocation.column, movedFigure->getColor(), movedFigure->getDirection());
		movedFigure = figureAt(newLocation);

		if (gameInteraction != nullptr)
		{
			gameInteraction->addFigureAt(newLocation.row, newLocation.column);
		}
		figures.removeFirstWhere([&](Figure* figureToDelete) {
			return oldField->getFigure() == figureToDelete;
		});

		delete oldField->getFigure();
		lastMoveDescriptor.didSpawnNewFigure = true;
	}
	else
	{
		lastMoveDescriptor.addRemovedFigure(newField->getFigure());
	}

	oldField->setFigure(nullptr);
	newField->setFigure(movedFigure);
}

void Board::revertUpdate(const Location& oldLocation, const Location& newLocation)
{
	Field* oldField = getFieldAt(oldLocation);
	Field* newField = getFieldAt(newLocation);
	if (oldField == nullptr || newField == nullptr)
	{
		return;
	}
	Figure* movedFigure = oldField->getFigure();

	oldField->setFigure(nullptr);
	newField->setFigure(movedFigure);
}

Figure* Board::getKing(ChessFigureColor color) const
{
	return figures.first([color](Figure* f) {
		return typeid(*f) == typeid(King) && f->getColor() == color;
	});
}

Field* Board::getField(Figure* figure) 
{
	return fields.first([&](Field* currentField) {
		return currentField->getFigure() == figure;
	});
}

List<Figure*> Board::remainingFigures(const ChessFigureColor& color) 
{
	auto condition = [color](Field* field)
	{
		return field != nullptr &&
			field->getFigure() != nullptr &&
			field->getFigure()->getColor() == color;
	};
	auto mutator = [](Field* field)
	{
		return field->getFigure();
	};

	//List<Field*> list = fields.filter(condition);
	//List<Figure*> figures = list.transformMap<Figure*>(mutator);

	return fields.filter(condition).transformMap<Figure*>(mutator);
}

List<List<Location>> Board::pawnDiagonalPossibleMoves(Figure* pawn, Location& location, List<List<Location>>& allMoves)
{
	List<List<Location>> filteredLocations;
	ChessFigureColor currentColor = pawn->getColor();
	int direction = static_cast<int>(pawn->getDirection());
	int diagonalRow = pow(-1, direction);
	Location leftD = Location(diagonalRow + location.row, location.column - 1);
	Location rightD = Location(diagonalRow + location.row, location.column + 1);
	Field* f1 = getFieldAt(leftD);
	Field* f2 = getFieldAt(rightD);

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

bool Board::movementIsPossibleInCastling(int fromRow, int fromCol, int toRow, int toCol)
{
	Location oldLocation(fromRow, fromCol);
	Field* field = getFieldAt(oldLocation);
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
	Field* newField = getFieldAt(newLocation);
	if (newField == nullptr)
	{
		return false;
	}
	Figure* otherFigure = newField->getFigure();

	if (field == nullptr || movedFigure || newField == nullptr)
	{
		newField->setFigure(movedFigure);
	}

	field->setFigure(nullptr);
	
	bool isInConflict = fieldIsInConflict(returnOpponentColor(movedFigure->getColor()));
	newField->setFigure(otherFigure);
	field->setFigure(movedFigure);
	return !isInConflict;
}

List<List<Location>> Board::filteredKingCastlingMoves(int row, int column, List<List<Location>>& allMoves)
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

List<Location> Board::filteredMoves(int row, int column)
{
	List<List<Location>> locations;

	Location givenLocation = Location(row, column);
	Figure* figure = figureAt(givenLocation);
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
			Field* f = getFieldAt(location);
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

bool Board::fieldIsInConflict(ChessFigureColor opponentColor)
{
	ChessFigureColor currentPlayerColor = opponentColor == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;
	Figure* king = getKing(currentPlayerColor);
	Field* kingField = getField(king);
    
    return fieldIsExposed(kingField->getLocation().row, kingField->getLocation().column, opponentColor);
}

bool Board::fieldIsExposed(int row, int col, ChessFigureColor color)
{
    // Check if the field is exposed by a knight
    int j {0};
    for(int i{1}; i <= 4; i++)
    {
        
        int newRow = row + 2 * pow(-1, i);
        j = i + j;
        int newCol = col + 1 * pow(-1, j);
        if(newRow >= ChessBoardRows || newCol >= ChessBoardColumns || newRow < 0 || newCol < 0)
            continue;
                    
        Location givenLocation = Location(newRow, newCol);
        Location mirroredLocation = Location(newCol, newRow);
        Figure* figureAtField = figureAt(givenLocation);
        if(figureAtField != nullptr  && typeid(*figureAtField) == typeid(Knight) && figureAtField->getColor() == color)
        {
            return true;
        }
        figureAtField = figureAt(mirroredLocation);
        if(figureAtField != nullptr  && typeid(*figureAtField) == typeid(Knight) && figureAtField->getColor() == color)
        {
            return true;
        }
    }
    
    vector<Location> expansion;
    expansion.push_back(Location(row + 1, col + 1));
    expansion.push_back(Location(row - 1, col - 1));
    expansion.push_back(Location(row + 1, col - 1));
    expansion.push_back(Location(row - 1, col + 1));
    
    expansion.push_back(Location(row + 1, col));
    expansion.push_back(Location(row - 1, col));
    expansion.push_back(Location(row, col - 1));
    expansion.push_back(Location(row, col + 1));
    
    bool firstLevelExpansion = true;
    while(!expansion.empty())
    {
        vector<Location> updatedExpansion;
        for(Location location : expansion)
        {
            if(location.row >= ChessBoardRows || location.column >= ChessBoardColumns || location.row < 0 || location.column < 0)
                continue;
            Figure* figureAtField = figureAt(location);
            if(figureAtField == nullptr)
            {
                int differenceInRow = location.row - row;
                int differenceInColumn = location.column - col;
                differenceInRow = differenceInRow > 1 ? 1 : differenceInRow;
                differenceInColumn = differenceInColumn > 1 ? 1 : differenceInColumn;
                
                differenceInRow = differenceInRow < -1 ? -1 : differenceInRow;
                differenceInColumn = differenceInColumn < -1 ? -1 : differenceInColumn;
                updatedExpansion.push_back(Location(location.row + differenceInRow, location.column + differenceInColumn));
            }
            else if(figureAtField->getColor() == color)
            {
                if(typeid(*figureAtField) == typeid(Pawn))
                {
                    int coefficient = figureAtField->getDirection() == ChessFigureDirection::Up ? 1 : -1;
                    bool initialFieldIsExposed = (location.row + coefficient == row && location.column + 1 == col) ||
                    (location.row + coefficient == row && location.column - 1 == col);
                   
                     if(initialFieldIsExposed)
                         return true;
                }
                else {
                    bool isTerminalQueen = typeid(*figureAtField) == typeid(Queen);
                    bool isTerminalBishop = typeid(*figureAtField) == typeid(Bishop) && (location.row != row && location.column != col);
                    bool isTerminalRook = typeid(*figureAtField) == typeid(Rook) && (location.row == row || location.column == col);
                    bool isTerminalKing = typeid(*figureAtField) == typeid(King) && firstLevelExpansion;
                    
                    if(isTerminalQueen || isTerminalBishop || isTerminalRook || isTerminalKing)
                    {
                        return true;
                    }
                }
            }
        }
        firstLevelExpansion = false;
        expansion = updatedExpansion;
    }
    return false;
}

List<Location> Board::filteredConflictMoves(List<Location> availableMoves, int row, int column)
{
	Location givenLocation = Location(row, column);
	Field* startingField = getFieldAt(givenLocation);
	Figure* figure = figureAt(givenLocation);
	ChessFigureColor opponentColor = figure->getColor() == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;

	if (startingField != nullptr) {
		startingField->setFigure(nullptr);
	}
	List<Location> allowedMoves = availableMoves.filter([&](Location currentLocation) {

		Field* newField = getFieldAt(currentLocation);
		Figure* oldFigure = newField->getFigure();
		newField->setFigure(figure);
		bool isConflict = fieldIsInConflict(opponentColor);
		newField->setFigure(oldFigure);
		return !isConflict;

	});
	startingField->setFigure(figure);
	return allowedMoves;
}

List<Location> Board::removeKingGapLocation(List<Location> location, int row, int column) {
	if ((row != KingRowBottom || column != KingColumn) && (row != KingRowTop || column != KingColumn))
		return location;

	Figure* figure = figureAt(Location(row, column));

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

	if (!leftFieldIsValid || conflict)
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

void Board::castlingPossible(int fromRow, int fromColumn)
{
	if (fromRow == RookBottomLeftRow && fromColumn == RookBottomLeftCol)
	{
		bottomLeftCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&bottomLeftCaslingIsPossible);
	}
	else if (fromRow == RookBottomRightRow && fromColumn == RookBottomRightCol)
	{
		bottomRightCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&bottomRightCaslingIsPossible);
	}
	else if (fromRow == RookTopLeftRow && fromColumn == RookTopLeftCol)
	{
		topLeftCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&topLeftCaslingIsPossible);
	}
	else if (fromRow == RookTopRightRow && fromColumn == RookTopRightCol)
	{
		topRightCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&topRightCaslingIsPossible);
	}
	else if (fromRow == KingRowBottom && fromColumn == KingColumn)
	{
		bottomLeftCaslingIsPossible = false;
		bottomRightCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&bottomRightCaslingIsPossible);
		lastMoveDescriptor.addFlagExistingRecord(&bottomLeftCaslingIsPossible);
	}
	else if (fromRow == KingRowTop && fromColumn == KingColumn)
	{
		topRightCaslingIsPossible = false;
		topLeftCaslingIsPossible = false;
		lastMoveDescriptor.addFlagExistingRecord(&topRightCaslingIsPossible);
		lastMoveDescriptor.addFlagExistingRecord(&topLeftCaslingIsPossible);
	}
}

void Board::moveRookInCastling(int fromRow, int fromColumn, int toRow, int toColumn) {
	Location oldLocation = Location(fromRow, fromColumn);
	Location newLocation = Location(toRow, toColumn);
	Field* field = getFieldAt(oldLocation);
	if (field == nullptr || field->getFigure() == nullptr)
		return;

	Figure* figure = field->getFigure();

	if (typeid(*figure) != typeid(King))
		return;

	int directionOfMovement = toColumn - fromColumn;
	if (directionOfMovement == 2)
	{
		Location initialRookLocation = Location(toRow, RookBottomRightCol);
		Location finalRookLocation = Location(toRow, RookBottomRightCol - ShortCastlingDistance);
		updateMove(initialRookLocation, finalRookLocation, true);
		if (gameInteraction != nullptr)
		{
			gameInteraction->move(toRow, RookBottomRightCol, toRow, RookBottomRightCol - ShortCastlingDistance, false);
		}
	}
	else if (directionOfMovement == -2)
	{
		Location initialRookLocation = Location(toRow, RookBottomLeftCol);
		Location finalRookLocation = Location(toRow, RookBottomLeftCol + LongCastlingDistance);
		updateMove(initialRookLocation, finalRookLocation, true);
		if (gameInteraction != nullptr)
		{
			gameInteraction->move(toRow, RookBottomLeftCol, toRow, RookBottomLeftCol + LongCastlingDistance, false);
		}
	}
}

List<Location> Board::availableMovesForFigure(int row, int column) {
	List<Location> initialFiltering = filteredMoves(row, column);
	List<Location> secondaryFilter = filteredConflictMoves(initialFiltering, row, column);
	return removeKingGapLocation(secondaryFilter, row, column);
}

List<Location> Board::availableMovesForFigure(Figure* figure)
{

	Field* currentField = getField(figure);
	if (currentField->getFigure() == nullptr)
		return List<Location>();

	Location currentLocation = currentField->getLocation();
	return availableMovesForFigure(currentLocation.row, currentLocation.column);
}

void Board::revertLastMove()
{
	List<MoveDescriptor> moves;
	try
	{
		moves = lastMoveDescriptor.popLastMoveDescriptors();
	}
	catch(exception e)
	{
		std::cout << "revertLastMove not possible, reason: " << e.what() << endl;
		return;
	}

	if (lastMoveDescriptor.didSpawnNewFigure && moves.size() == 1)
	{
		MoveDescriptor move = moves[0];
		if (figureAt(move.to) != nullptr)
		{
			Location newLocation = move.to;
			Figure* oldFigure = figureAt(newLocation);
			ChessFigureColor color = oldFigure->getColor();
			ChessFigureDirection direction = oldFigure->getDirection();
			figures.removeFirstWhere([&](Figure* figureToDelete) {
				return oldFigure == figureToDelete;
			});
			delete oldFigure;

			if (gameInteraction != nullptr)
			{
				gameInteraction->removeFigureAt(newLocation.row, newLocation.column);
			}

			pawnFactory(newLocation.row, newLocation.column, color, direction);
			int key = getKeyForLocation(newLocation.row, newLocation.column);
			Field* field = fieldsMap[key];
			if (field != nullptr)

			if (gameInteraction != nullptr)
			{
				gameInteraction->addFigureAt(newLocation.row, newLocation.column);
			}
			lastMoveDescriptor.didSpawnNewFigure = false;
		}
	}

	for (int i{ 0 }; i < moves.size(); i++)
	{
		MoveDescriptor currentPair = moves[i];
		Location toLocation = currentPair.to;
		Location fromLocation = currentPair.from;

		revertUpdate(toLocation, fromLocation);
		if (gameInteraction != nullptr)
		{
			gameInteraction->move(toLocation.row, toLocation.column, fromLocation.row, fromLocation.column, false);
		}
	}

	List<bool*> flags = lastMoveDescriptor.popLastFlags();

	for (int i{ 0 }; i < flags.size(); i++)
	{
		bool* currentFlag = flags[i];
		*currentFlag = !(*currentFlag);
	}

	Figure* lastRemovedFigure = lastMoveDescriptor.popLastRemovedFigure();

	if (moves.size() == 1 && lastRemovedFigure != nullptr)
	{
		Field* field = getFieldAt(moves[0].to);
		field->setFigure(lastRemovedFigure);
		if (gameInteraction != nullptr)
		{
			gameInteraction->addFigureAt(field->getLocation().row, field->getLocation().column);
		}
	}
}

bool Board::getLastMoveWasCastling() const
{
	return lastMoveWasCastling;
}

List<MoveDescriptor> Board::possibleMovesForColor(ChessFigureColor color)
{
	List<MoveDescriptor> result;
	List<Figure*> figures = remainingFigures(color);

	figures.forEach([&](Figure* figure) {

		Location figureLocation = getField(figure)->getLocation();
		List<Location> availableMoves = availableMovesForFigure(figure);
		availableMoves.forEach([&](Location location) {

			MoveDescriptor newMoveDescriptor(figureLocation, location);
			result.pushFront(newMoveDescriptor);

		});
	});
	return result;
}
