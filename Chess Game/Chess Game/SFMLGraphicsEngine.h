#pragma once
#include "GraphicsEngine.h"
#include <SFML/Graphics.hpp>

class SFMLGraphicsEngine : public GraphicsEngine
{
	double figureBoxSize;
	void populateFigures();
public:
	void initiateRender();
	bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate = true);
};