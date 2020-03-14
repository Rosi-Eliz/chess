#pragma once

#include <vector>
#include <iostream>
using namespace std;

// Base interface definition

enum Figure {Rook, Knight, Bishop, Queen, King, Pawn};
enum FigureType {Black, White};
enum BoardLayout {LeadingWhites, LeadingBlacks};

// TODO: Update all row and column parrameters to be used as a part of the Location struct
struct Location {
	int row;
	int column;
};

class GraphicsEngine {

	virtual void addFigure(Figure figure, FigureType figureType, int row, int column) = 0;
	virtual void initiateRender(BoardLayout boardLayout) = 0;
	virtual bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) = 0;
	virtual bool removeFigure(int row, int column) = 0;
	virtual void addPossibleMoveSquare(int row, int column) = 0;
	virtual void removePossibleMoves() = 0;

public:
	vector<Location> (*availableMovesForFigure)(int row, int column);
	bool (*isMoveValid)(int fromRow, int fromColumn, int toRow, int toColumn);
	void (*didRemoveFigure)(int row, int column);
	void (*didMove)(int fromRow, int fromColumn, int toRow, int toColumn);
};
