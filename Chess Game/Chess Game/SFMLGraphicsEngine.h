#pragma once
#include "GraphicsEngine.h"
#include <SFML/Graphics.hpp>

class SFMLGraphicsEngine : public GraphicsEngine
{
	float figureBoxSize;
	void populateFigures();
public:
	void initiateRender();
};