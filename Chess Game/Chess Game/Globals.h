#pragma once
#include "List.h"

static const int ChessBoardRows = 8;
static const int ChessBoardColumns = 8;

static const int SupplementaryRowDownDirection = 7;
static const int SupplementaryRowUpDirection = 0;
static const int InitialRowDownDirection = 6;
static const int InitialRowUpDirection = 1;
static const int InitialColumn = 0;
static const int EndColumn = 7;

static const int KingRowTop = 7;
static const int KingRowBottom = 0;
static const int KingColumn = 4;


static const int RookBottomLeftRow = 0;
static const int RookBottomLeftCol = 0;
static const int RookBottomRightRow = 0;
static const int RookBottomRightCol = 7;
static const int RookTopLeftRow = 7;
static const int RookTopLeftCol = 0;
static const int RookTopRightRow = 7;
static const int RookTopRightCol = 7;

static const int LongCastlingDistance = 3;
static const int ShortCastlingDistance = 2;

enum class State { Draw, WhiteWins, BlackWins, InProgress };
enum class ChessFigureColor { Black, White };
enum class ChessBoardLayout { LeadingBlacks, LeadingWhites };
enum class ChessFigureDirection { Up, Down };



struct Location {
	int row;
	int column;

	Location() {}
	Location(const Location& location)
	{
		row = location.row;
		column = location.column;
	}
	Location(int row, int column) : row(row), column(column) {}

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
    bool operator==(const Location& location)
    {
        return row == location.row && column == location.column;
    }
};

static ChessFigureColor returnOpponentColor(ChessFigureColor color)
{
	ChessFigureColor oppositePlayerColor = color == ChessFigureColor::White ? ChessFigureColor::Black : ChessFigureColor::White;
	return oppositePlayerColor;
}

static int getKeyForLocation(int row, int col)
{
	return 10 * row + col;
}

static Location getLocationForKey(int key)
{
	Location location = Location(key / 10, key % 10);
	return location;
}
