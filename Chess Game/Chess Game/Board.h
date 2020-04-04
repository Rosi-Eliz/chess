#pragma once
#include "List.h"
#include "Field.h"
#include "Globals.h"
#include "Figure.h"
#include "GameInteraction.h"

class Board {
	List<Field*> fields;
	ChessBoardLayout layout;
	List<Figure*> figures;

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
	void updateMove(const Location& oldLocation,const Location& newLocation);
	ChessBoardLayout getLayout() const;
	Figure* figureAt(const Location& location);
	Field* getFieldAt(const Location& location);
	List<Field*> getFields() const;
	List<Figure*> remainingFigures(const ChessFigureColor& color);
	Figure* getKing(ChessFigureColor color) const;
	Field* getField(Figure* figure) const;
};