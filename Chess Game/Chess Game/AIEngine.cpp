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

double AIEngine::evaluateBoard(Board* board, ChessFigureColor color, bool isMaxPlayer)
{
	double points{ 0 };

	List<Figure*> figures = board->remainingFigures(color);
	for (int i{0}; i < figures.size(); i ++)
	{
		Location location = board->getField(figures[i])->getLocation();
		points += figures[i]->getValueForPosition(location);
	}

	if (board->getLastMoveWasCastling())
		points += 4;

	int opponentMoves = board->possibleMovesForColor(returnOpponentColor(color)).size();
	int ourMoves = board->possibleMovesForColor(color).size();

	if (opponentMoves == 0 && ourMoves == 0)
		points = 0;

	else if (board->fieldIsInConflict(returnOpponentColor(color)))
	{

		if (opponentMoves == 0)
			points = INFINITY;

		else
			points += 2; //Declared check 
	
	}

	return isMaxPlayer ? points : -points;
}