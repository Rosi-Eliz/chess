#pragma once

#define ChessBoardRows 8;
#define ChessBoardColumns 8;

enum class ChessFigureColor {Black, White};

enum class ChessFigureDirection { Up, Down };

struct Location {
	int row;
	int column;
};