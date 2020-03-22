#include "Game.h"
#include "Bishop.h"
#include "King.h"


Game::Game() {
	King p(ChessFigureColor::White, ChessFigureDirection::Up);

	Location l;
	l.row = 0;
	l.column = 0;
	List<List<Location>> list = p.possibleMoves(l);

	Location::printNestedLocations(list);
}