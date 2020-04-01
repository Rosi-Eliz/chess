#pragma once

class GameInteraction {
public:
	virtual void removeFigureAt(int row, int col) {};
	virtual void addFigureAt(int row, int col) {};
};