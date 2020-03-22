// Chess Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SFMLGraphicsEngine.h"
#include <iostream>
#include "Game.h"

// We are emulating an interface based structure, which is not supported in C++
// so as to achieve a cross-platform application, therefore only GraphicsEngine members
// should be accessed/modified from here.
static SFMLGraphicsEngine graphicsEngine = SFMLGraphicsEngine();

std::vector<Location> availableMovesForFigure(int row, int column) {
	Location a1, a2;
	a1.row = 3;
	a1.column = 3;
	a2.row = 3;
	a2.column = 4;

	return {a1, a2};
}

void didRemoveFigure(int row, int column) {
	cout << "removed: row: "<< row << ", column: " << column << endl;
}

bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) {
	return toRow == 3 && (toColumn == 3 || toColumn == 4);
}

void didMove(int fromRow, int fromColumn, int toRow, int toColumn) {
	cout << "move from - row: " << fromRow << ", column: " << fromColumn << ". To - row: ";
	cout << toRow << ", column: " << toColumn << endl;
}

bool isPlayerActive(FigureType figureType) {
	return figureType == White;
}

int main()
{
	Game g;
	graphicsEngine.availableMovesForFigure = availableMovesForFigure;
	graphicsEngine.didRemoveFigure = didRemoveFigure;
	graphicsEngine.isMoveValid = isMoveValid;
	graphicsEngine.didMove = didMove;
	graphicsEngine.isPlayerActive = isPlayerActive;
	graphicsEngine.initiateRender();
	return 0;
}