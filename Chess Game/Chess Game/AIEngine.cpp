#include "AIEngine.h"
#include <climits>
#include <chrono>
AIEngine::AIEngine()
{
}

AIEngine::AIEngine(Difficulty difficulty, ChessFigureColor color, AIUpdateProvider* updateProvider)
{
	this->difficulty = difficulty;
	this->color = color;
	this->updateProvider = updateProvider;
	this->initialColor = color;
}

int depthForDifficulty(Difficulty difficulty)
{
	switch (difficulty)
	{
		case Difficulty::Beginner: return 2;
		case Difficulty::Intermediate: return 3;
		case Difficulty::Advanced: return 4;
		case Difficulty::Expert: return 5;
	}
	return 0;
}

void AIEngine::findBestMoveIn(Board board)
{
	this->board = board;
	this->board.gameInteraction = nullptr;

	maximizer(depthForDifficulty(difficulty), INT_MIN, INT_MAX);
	if (bestFrom.row != -1 && bestFrom.column != -1 && bestTo.row != -1 && bestTo.column != -1)
	{
		updateProvider->didChooseBestMove(bestFrom.row, bestFrom.column, bestTo.row, bestTo.column);
	}
}

double AIEngine::evaluateBoard(ChessFigureColor color, bool isMinPlayer)
{
	double points{ 0 };

	List<Figure*> figures = board.allRemainingFigures();
	for (int i{0}; i < figures.size(); i ++)
	{
		Location location = board.getField(figures[i])->getLocation();
		int coefficient = color == figures[i]->getColor() ? 1 : -1;
		points += figures[i]->getValueForPosition(location) * coefficient;

		// Figure protection
		if (board.fieldIsExposed(location.row, location.column, color) && color == figures[i]->getColor())
		{
			points += 0.25;
		}
	}

	if (board.getLastMoveWasCastling())
		points += 4;

	int opponentMoves = board.possibleMovesForColor(returnOpponentColor(color)).size();
	int ourMoves = board.possibleMovesForColor(color).size();

	points += static_cast<double>(ourMoves-opponentMoves) * 0.25;

	if (opponentMoves == 0 && ourMoves == 0)
		points = 0;

	else if (board.fieldIsInConflict(returnOpponentColor(color)))
	{

		if (opponentMoves == 0)
			points = INFINITY;

		else
			points += 4; //Declared check 
	
	}
	return isMinPlayer ? -points : points;
}

int AIEngine::maximizer(int depth, int alpha, int beta)
{
	if (depth == 0)
	{
		return evaluateBoard(color, false);
	}

	board.remainingFigures(color).forEach([&](Figure* figure) {
		Location fromLocation = board.getField(figure)->getLocation();
		board.availableMovesForFigure(figure).forEach([&](Location toLocation) {
			board.updateMove(fromLocation, toLocation);
			color = returnOpponentColor(color);
			int evaluation = minimizer(depth - 1, alpha, beta);
			color = returnOpponentColor(color);
			board.revertLastMove();

			if (evaluation > alpha)
			{
				alpha = evaluation;
				if (depth == depthForDifficulty(difficulty))
				{
					bestFrom = fromLocation;
					bestTo = toLocation;
				}
			}

			if (alpha >= beta)
			{
				return alpha;
			}
		});
	});
	return alpha;
}

int AIEngine::minimizer(int depth, int alpha, int beta)
{
	if (depth == 0)
	{
		return evaluateBoard(color, true);
	}

	auto t_start = std::chrono::high_resolution_clock::now();
	List<Figure*> remainingFigures = board.remainingFigures(color);
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	//cout << "rem fig time: " << elapsed_time_ms << endl;
    remainingFigures.forEach([&](Figure* figure) {

		t_start = std::chrono::high_resolution_clock::now();
		Location fromLocation = board.getField(figure)->getLocation();
		List<Location> availableMoves = board.availableMovesForFigure(figure);
		t_end = std::chrono::high_resolution_clock::now();
		elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		//cout << "availableMoves time: " << elapsed_time_ms << endl;

		availableMoves.forEach([&](Location toLocation) {

			board.updateMove(fromLocation, toLocation);
			color = returnOpponentColor(color);
			int evaluation = maximizer(depth - 1, alpha, beta);
			color = returnOpponentColor(color);
			board.revertLastMove();

			if (evaluation <= beta)
			{
				beta = evaluation;
			}

			if (alpha >= beta)
			{
				return beta;
			}
		});
	});

	return beta;
}
