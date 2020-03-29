#include "Board.h"
#include "Figure.h"
#include "Rook.h"
#include "Field.h"
#include "Bishop.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"

Board::Board(ChessBoardLayout layout) : layout(layout)
{
	initialiseFields();
	initialiseFigures();
}

Board::Board() {
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
	kingFactory(SupplementaryRowDownDirection, 3, topFiguresColor, ChessFigureDirection::Down);
	queenFactory(SupplementaryRowDownDirection, 4, topFiguresColor, ChessFigureDirection::Down);
	bishopFactory(SupplementaryRowDownDirection, 5, topFiguresColor, ChessFigureDirection::Down);
	knightFactory(SupplementaryRowDownDirection, 6, topFiguresColor, ChessFigureDirection::Down);
	rookFactory(SupplementaryRowDownDirection, 7, topFiguresColor, ChessFigureDirection::Down);
	//Initialize top pawns row 
	for (int i{ 0 }; i < 8; i++)
	{
		pawnFactory(InitialRowDownDirection, i, topFiguresColor, ChessFigureDirection::Down);
	}

	//Intialize special figures row

	rookFactory(SupplementaryRowUpDirection, 0, bottomFiguresColor, ChessFigureDirection::Up);
	knightFactory(SupplementaryRowUpDirection, 1, bottomFiguresColor, ChessFigureDirection::Up);
	bishopFactory(SupplementaryRowUpDirection, 2, bottomFiguresColor, ChessFigureDirection::Up);
	kingFactory(SupplementaryRowUpDirection, 3, bottomFiguresColor, ChessFigureDirection::Up);
	queenFactory(SupplementaryRowUpDirection, 4, bottomFiguresColor, ChessFigureDirection::Up);
	bishopFactory(SupplementaryRowUpDirection, 5, bottomFiguresColor, ChessFigureDirection::Up);
	knightFactory(SupplementaryRowUpDirection, 6, bottomFiguresColor, ChessFigureDirection::Up);
	rookFactory(SupplementaryRowUpDirection, 7, bottomFiguresColor, ChessFigureDirection::Up);
	//Initialize bottom pawns row 
	for (int i{ 0 }; i < 8; i++)
	{
		pawnFactory(InitialRowUpDirection, i, bottomFiguresColor, ChessFigureDirection::Up);
	}
}

Figure* Board::figureAt(const Location& location) 
{
	for (int i{ 0 }; i < fields.size(); i++)
	{
		if (fields[i]->getLocation().row == location.row && 
			fields[i]->getLocation().column == location.column)
		{
			return fields[i]->getFigure();
		}
	}
	return nullptr;
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
	for (int i{ 0 }; i < fields.size(); i++)
	{
		if (fields[i]->getLocation().row == location.row && 
			fields[i]->getLocation().column == location.column)
		{
			return fields[i];
		}
	}
	return nullptr;
}

