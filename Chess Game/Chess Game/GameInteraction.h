#pragma once

class GameInteraction {
public:

	virtual void removeFigureAt(int row, int col) = 0;
	virtual void addFigureAt(int row, int col) = 0;
	virtual void move(int fromRow, int fromCol, int toRow, int toCol) = 0;
};