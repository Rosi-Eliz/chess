// Chess Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Globals.h"
#include "Game.h"

int main()
{
	Game game(ChessBoardLayout::LeadingWhites);
	game.initiateGame();
	return 0;
}