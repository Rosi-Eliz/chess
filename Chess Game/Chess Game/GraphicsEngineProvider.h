#pragma once

struct Location;

enum FigureDesignation { RookDesignation, KnightDesignation, BishopDesignation, QueenDesignation, KingDesignation, PawnDesignation };
enum FigureType { Black, White };
enum BoardLayout { LeadingWhites, LeadingBlacks };

class GraphicsEngineProvider {

public:
	virtual List<Location>availableMovesForFigure(int row, int column) = 0;
	virtual bool isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) = 0;
	virtual void didRemoveFigure(int row, int column) = 0;
	virtual void didMove(int fromRow, int fromColumn, int toRow, int toColumn) = 0;
	virtual bool isPlayerActive(FigureType figureType) = 0;
	virtual void didRestartGame() = 0;
};