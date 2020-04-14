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

}