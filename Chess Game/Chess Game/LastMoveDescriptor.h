#pragma once
#include "List.h"
#include "Globals.h"
#include "MoveDescriptor.h"

struct LastMoveDescriptor {
private:
	List<List<MoveDescriptor>> moveRecords;
public:

	List<bool*> changedCastlingFlags;
	bool didSpawnNewFigure = false;

	void addMoveNewRecord(MoveDescriptor moveDescriptor)
	{
		List<MoveDescriptor> list;
		list.pushFront(moveDescriptor);
		moveRecords.pushFront(list);
		//list.pushFront(moveDescriptor);
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
	void deleteMoves()
	{
		moves.forEach([&](List<MoveDescriptor*> pair) {
			pair.forEach([&](MoveDescriptor* moveDescriptor) {
				delete moveDescriptor;
				});
			});
	}
	*/
	void clear() {
		//deleteMoves();
		moveRecords = List<List<MoveDescriptor>>();
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
};