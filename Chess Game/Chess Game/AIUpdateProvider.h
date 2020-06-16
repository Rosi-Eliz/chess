#pragma once


class AIUpdateProvider
{
public:
	virtual void didChooseBestMove(int fromRow, int fromCol, int toRow, int toCol) = 0;
};