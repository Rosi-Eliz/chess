#pragma once
#include "List.h"
#include "Globals.h"
#include "MoveDescriptor.h"

struct LastMoveDescriptor {
private:
	List<List<MoveDescriptor>> moveRecords;
	List<List<bool*>> changedCastlingFlags;
public:

	
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

	void addFlagNewRecord(bool* flag)
	{
		List<bool*> flagList;
		flagList.pushFront(flag);
		changedCastlingFlags.pushFront(flagList);

	}

	void addFlagExistingRecord(bool* flag)
	{
		if (changedCastlingFlags.isEmpty())
			addFlagNewRecord(flag);
		else
		{
			//if no such flag exists already
			if (!changedCastlingFlags[changedCastlingFlags.size() - 1].contains([&](bool* storedFlag) {
				return storedFlag == flag;
			}))
			{
				changedCastlingFlags[changedCastlingFlags.size() - 1].pushFront(flag);
			}
		}
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