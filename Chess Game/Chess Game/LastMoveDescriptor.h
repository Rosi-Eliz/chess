#pragma once
#include "List.h"
#include "Globals.h"
#include "MoveDescriptor.h"
#include "Figure.h"

struct LastMoveDescriptor {
private:
	List<List<MoveDescriptor>> moveRecords;
	List<List<bool*>> changedCastlingFlags;
	List<Figure*> removedFigures;
public:

	bool didSpawnNewFigure = false;

	void addMoveNewRecord(MoveDescriptor moveDescriptor)
	{
		List<MoveDescriptor> list;
		list.pushFront(moveDescriptor);
		moveRecords.pushFront(list);
		
		List<bool*> flags;
		changedCastlingFlags.pushFront(flags);
		removedFigures.pushFront(nullptr);
	}

	void addMoveExistingRecord(MoveDescriptor moveDescriptor)
	{
		if (!moveRecords.isEmpty())
		{
			moveRecords[moveRecords.size() - 1].pushFront(moveDescriptor);
		}
		else
		{
			addMoveNewRecord(moveDescriptor);
		}
	}

	List<MoveDescriptor> popLastMoveDescriptors()
	{
		if (moveRecords.isEmpty())
		{
			throw runtime_error("No records left");
		}
		int lastIndex = moveRecords.size() - 1;
		List<MoveDescriptor> poppedElements = moveRecords[lastIndex];
		moveRecords.removeAt(lastIndex);

		return poppedElements;
	}

	/*
	void addFlagNewRecord(bool* flag)
	{
		List<bool*> flagList;
		flagList.pushFront(flag);
		changedCastlingFlags.pushFront(flagList);

	}
    */

	void addFlagExistingRecord(bool* flag)
	{
		if (changedCastlingFlags.isEmpty())
			throw runtime_error("flags should not be empty");
		else
		{
			List<bool*>& flags = changedCastlingFlags[changedCastlingFlags.size() - 1];

			//if no such flag exists already
			if (!flags.contains([&](bool* storedFlag) {
				return storedFlag == flag;
			}))
			{
				flags.pushFront(flag);
			}
		}
	}

	void addRemovedFigure(Figure* figure)
	{
		if (removedFigures.isEmpty())
		{
			throw runtime_error("removed figures should not be empty");
		}
		removedFigures[removedFigures.size() - 1] = figure;
	}

	Figure* popLastRemovedFigure()
	{
		if (removedFigures.isEmpty())
		{
			throw runtime_error("removed figures list is empty!");
		}

		Figure* lastRemovedFigure = removedFigures[removedFigures.size() - 1];
		removedFigures.removeAt(removedFigures.size() - 1);
		return lastRemovedFigure;
	}

	List<bool*> popLastFlags()
	{
		if (changedCastlingFlags.isEmpty())
			throw runtime_error("Flag list is empty!");

		int lastIndex = changedCastlingFlags.size() - 1;
		List<bool*> poppedElements = changedCastlingFlags[lastIndex];
		changedCastlingFlags.removeAt(lastIndex);

		return poppedElements;
	}

	void clear() {
		//deleteMoves();
		moveRecords = List<List<MoveDescriptor>>();
		changedCastlingFlags = List<List<bool*>>();
		didSpawnNewFigure = false;
	};
};