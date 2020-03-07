// Chess Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SFMLGraphicsEngine.h"

// We are emulating an interface based structure, which is not supported in C++
// so as to achieve a cross-platform application, therefore only GraphicsEngine members
// should be accessed/modified from here.
static SFMLGraphicsEngine graphicsEngine = SFMLGraphicsEngine();

int main()
{
	graphicsEngine.initiateRender();
	return 0;
}