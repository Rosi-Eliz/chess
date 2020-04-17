#pragma once
#include "List.h"
#include "Field.h"
#include "Globals.h"
#include "Figure.h"
#include "GameInteraction.h"

struct Pair {

	Location from;
	Location to;
	Pair(Location from, Location to) : from(from), to(to) {}
	Pair(const Pair& pair) { *this = pair; }
	Pair& operator=(const Pair& pair) {
		if (this == &pair)
			return *this;

		from = pair.from; 
		to = pair.to; 
		return *this;
	}
};

struct LastMoveDescriptor {

	List<Pair*> moves;
	List<bool*> changedCastlingFlags;
	void clear() {
		moves.forEach([&](Pair* pair) {
			delete pair;
		});
		moves = List<Pair*>();
		changedCastlingFlags = List<bool*>();
	};

	~LastMoveDescriptor()
	{
		moves.forEach([&](Pair* pair) {
			delete pair;
		});
	}
};

class Board {
	List<Field*> fields;
	ChessBoardLayout layout;
	List<Figure*> figures;
	LastMoveDescriptor lastMoveDescriptor;

	bool bottomRightCaslingIsPossible = true;
	bool bottomLeftCaslingIsPossible = true;
	bool topRightCaslingIsPossible = true;
	bool topLeftCaslingIsPossible = true;

	bool movementIsPossibleInCastling(int fromRow, int fromCol, int toRow, int toCol);
	List<Location>filteredMoves(int row, int column);
	List<Location>filteredConflictMoves(List<Location> availableMoves, int row, int column);
	List<Location>removeKingGapLocation(List<Location> location, int row, int column);
	List<List<Location>> pawnDiagonalPossibleMoves(Figure* pawn, Location& location, List < List<Location>>& allMoves);
	List<List<Location>> filteredKingCastlingMoves(int row, int column, List<List<Location>>& allMoves);

	void rookFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void bishopFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void knightFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void queenFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void kingFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void pawnFactory(int row, int col, ChessFigureColor color, ChessFigureDirection direction);
	void initialiseFields();
	void initialiseFigures();
	void assignFigureToField(Location& location, Figure* figiure);
	GameInteraction* gameInteraction;

public: 
	Board(ChessBoardLayout layout, GameInteraction* gameInteraction);
	Board();
	Board(const Board& board);
	Board& operator=(const Board& board);
	~Board();
	void updateMove(const Location& oldLocation,const Location& newLocation);
	bool fieldIsInConflict(ChessFigureColor opponentColor);
	ChessBoardLayout getLayout() const;
	Figure* figureAt(const Location& location);
	Field* getFieldAt(const Location& location);
	List<Field*> getFields() const;
	List<Figure*> remainingFigures(const ChessFigureColor& color);
	Figure* getKing(ChessFigureColor color) const;
	Field* getField(Figure* figure) const;
	List<Location>availableMovesForFigure(int row, int column);
	List<Location> availableMovesForFigure(Figure* figure);
	void castlingPossible(int fromRow, int fromColumn);
	void moveRookInCastling(int fromRow, int fromColumn, int toRow, int toColumn);
	void revertLastMove(bool shouldRenderChanges);
};