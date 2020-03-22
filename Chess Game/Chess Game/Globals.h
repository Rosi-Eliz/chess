#pragma once
#include "List.h"

static const int ChessBoardRows = 8;
static const int ChessBoardColumns = 8;

enum class ChessFigureColor {Black, White};

enum class ChessFigureDirection { Up, Down };

struct Location {
	int row;
	int column;

	static void printNestedLocations(List<List<Location>> list)
	{
		for (int i{ 0 }; i < list.size(); i++)
		{
			List<Location> nestedList = list[i];
			for (int j{ 0 }; j < nestedList.size(); j++) {

				Location location = nestedList[j];
				std::cout << "{ row: " << location.row << ", column: " << location.column << " } ";
			}
			std::cout << std::endl;
		}
	}
};