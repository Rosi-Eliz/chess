#pragma once
#include "Game.h"
#include "Board.h"
#include "Globals.h"
#include "AIUpdateProvider.h"

enum class Difficulty {Beginner, Intermediate, Advanced, Expert};


class AIEngine {
	Difficulty difficulty;
	ChessFigureColor color;
	AIUpdateProvider* updateProvider;

public:
	AIEngine();
	AIEngine(Difficulty difficulty, ChessFigureColor color, AIUpdateProvider* updateProvider);
	void findBestMoveIn(Board board);
};


