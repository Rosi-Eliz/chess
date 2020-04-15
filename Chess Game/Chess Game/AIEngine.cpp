#include "AIEngine.h"

AIEngine::AIEngine()
{
}

AIEngine::AIEngine(Difficulty difficulty, ChessFigureColor color, AIUpdateProvider* updateProvider)
{
	this->difficulty = difficulty;
	this->color = color;
	this->updateProvider = updateProvider;
}

void AIEngine::findBestMoveIn(Board board)
{
	List<Figure*> figures = board.remainingFigures(color);
	List<Figure*> moreThanOne = figures.filter([&](Figure* figure) {
		//return figure->possibleMoves(board.get).size() > 0;
		return true;
	});
}