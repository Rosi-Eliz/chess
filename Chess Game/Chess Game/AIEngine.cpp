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
		return board.availableMovesForFigure(figure).size() > 0;
	});

	if (moreThanOne.size() == 0)
	{
		return;
	}

	int randomIndex = rand() % moreThanOne.size();
	Figure* figure = moreThanOne[randomIndex];
	Location figureLocation = board.getField(figure)->getLocation();
	List<Location> figureMoves = board.availableMovesForFigure(figure);
	Location location = figureMoves[rand() % figureMoves.size()];
	updateProvider->didChooseBestMove(figureLocation.row, figureLocation.column, location.row, location.column);

}