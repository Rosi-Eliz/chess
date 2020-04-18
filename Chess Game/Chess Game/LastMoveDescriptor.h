#pragma once
#include "List.h"
#include "Globals.h"
#include "MoveDescriptor.h"

struct LastMoveDescriptor {

	List<MoveDescriptor*> moves;
	List<bool*> changedCastlingFlags;
	bool didSpawnNewFigure = false;
	void clear() {
		moves.forEach([&](MoveDescriptor* pair) {
			delete pair;
			});
		moves = List<MoveDescriptor*>();
		changedCastlingFlags = List<bool*>();
		didSpawnNewFigure = false;
	};

	void addFlag(bool* flag)
	{
		if (!changedCastlingFlags.contains([&](bool* storedFlag) {
			return storedFlag == flag;
			}))
		{
			changedCastlingFlags.pushFront(flag);
		}
	}

	~LastMoveDescriptor()
	{
		moves.forEach([&](MoveDescriptor* pair) {
			delete pair;
			});
	}
};