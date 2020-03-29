#pragma once

#include <vector>
#include <iostream>
#include "Globals.h"
#include "List.h"
using namespace std;

// Base interface definition

enum FigureDesignation {RookDesignation, KnightDesignation, BishopDesignation, QueenDesignation, KingDesignation, PawnDesignation};
enum FigureType {Black, White};
enum BoardLayout {LeadingWhites, LeadingBlacks};

// TODO: Update all row and column parrameters to be used as a part of the Location struct

class GraphicsEngine {

	virtual void addFigure(FigureDesignation figure, FigureType figureType, int row, int column) = 0;
	virtual void initiateRender(BoardLayout boardLayout) = 0;
	virtual bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) = 0;
	virtual bool removeFigure(int row, int column) = 0;
	virtual void addPossibleMoveSquare(int row, int column) = 0;
	virtual void removePossibleMoves() = 0;

public:
	List<Location> (*availableMovesForFigure)(int row, int column);
	bool (*isMoveValid)(int fromRow, int fromColumn, int toRow, int toColumn);
	void (*didRemoveFigure)(int row, int column);
	void (*didMove)(int fromRow, int fromColumn, int toRow, int toColumn);
	bool (*isPlayerActive)(FigureType figureType);
};
