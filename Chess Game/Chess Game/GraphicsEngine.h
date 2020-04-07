#pragma once

#include <vector>
#include <iostream>
#include "Globals.h"
#include "List.h"
#include "GraphicsEngineProvider.h"
using namespace std;

// Base interface definition

// TODO: Update all row and column parrameters to be used as a part of the Location struct

class GraphicsEngine {

	virtual void addFigure(FigureDesignation figure, FigureType figureType, int row, int column) = 0;
	virtual void initiateRender(BoardLayout boardLayout) = 0;
	virtual bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) = 0;
	virtual bool removeFigure(int row, int column) = 0;
	virtual void addPossibleMoveSquare(int row, int column) = 0;
	virtual void addLastMoveSquare(int row, int column) = 0;
	virtual void removePossibleMoves() = 0;
	virtual void removeAllFigures() = 0; 
	virtual void showGameOverLayout() = 0;
	virtual void hideGameOverLayout() = 0;
};
