#include "Game.h"
#include "Pawn.h"


Game::Game() {
	Pawn p(ChessFigureColor::White, ChessFigureDirection::Up);

	Location l;
	l.row = 1;
	l.column = 0;
	List<List<Location>> list = p.possibleMoves(l);
}