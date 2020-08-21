#pragma once
#include "Board.h"
#include "AIUpdateProvider.h"

enum class Difficulty {Beginner, Intermediate, Advanced, Expert};


class AIEngine {
	Difficulty difficulty;
	ChessFigureColor color;

	AIUpdateProvider* updateProvider;
	Board board;

	Location bestFrom = Location(-1, -1);
	Location bestTo = Location(-1, -1);

	int maximizer(int depth, int alpha, int beta);
	int minimizer(int depth, int alpha, int beta);

public:
    ChessFigureColor initialColor;
	AIEngine();
	AIEngine(Difficulty difficulty, ChessFigureColor color, AIUpdateProvider* updateProvider);
	void findBestMoveIn(Board board);

	double evaluateBoard(ChessFigureColor color, bool isMinPlayer);
};


