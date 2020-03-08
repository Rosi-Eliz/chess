#pragma once

// Base interface definition

class GraphicsEngine {
	virtual void initiateRender() = 0;
	virtual bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) = 0;
	virtual bool removeFigure(int row, int column) = 0;
};
