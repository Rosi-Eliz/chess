#pragma once
#include "List.h"
#include "Field.h"
#include "Globals.h"
#include "Figure.h"

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
public: 
	Board(ChessBoardLayout layout);
	Board();
	ChessBoardLayout getLayout() const;
	Figure* figureAt(const Location& location);
	Field* getFieldAt(const Location& location);
	List<Field*> getFields() const;
};