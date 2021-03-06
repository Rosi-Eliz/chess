#pragma once
#include <unordered_map>
#include "List.h"
#include "Field.h"
#include "Globals.h"
#include "Figure.h"
#include "GameInteraction.h"
#include "LastMoveDescriptor.h"



class Board {
	List<Field*> fields;
	ChessBoardLayout layout;
	List<Figure*> figures;
	LastMoveDescriptor lastMoveDescriptor;
	std::unordered_map<int, Field*> fieldsMap;
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
	void revertUpdate(const Location& oldLocation, const Location& newLocation);

	bool lastMoveWasCastling = false;
public: 
	Board(ChessBoardLayout layout, GameInteraction* gameInteraction);
	Board();
	Board(const Board& board);
	Board& operator=(const Board& board);
	~Board();
	void updateMove(const Location& oldLocation,const Location& newLocation, bool didPerformCastling = false);
	
	bool fieldIsInConflict(ChessFigureColor opponentColor);
    bool fieldIsExposed(int row, int col, ChessFigureColor color);
	ChessBoardLayout getLayout() const;
	Figure* figureAt(const Location& location);
	Field* getFieldAt(const Location& location);
	List<Field*> getFields() const;
	List<Figure*> remainingFigures(const ChessFigureColor& color);
	Figure* getKing(ChessFigureColor color) const;
	Field* getField(Figure* figure);
	List<Location>availableMovesForFigure(int row, int column);
	List<Location> availableMovesForFigure(Figure* figure);
	void castlingPossible(int fromRow, int fromColumn);
	void moveRookInCastling(int fromRow, int fromColumn, int toRow, int toColumn);
	bool getLastMoveWasCastling() const;
	void revertLastMove();
	GameInteraction* gameInteraction;
	List<Figure*> allRemainingFigures();

	List<MoveDescriptor> possibleMovesForColor(ChessFigureColor color);
};
