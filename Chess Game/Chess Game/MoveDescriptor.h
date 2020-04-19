#pragma once

struct MoveDescriptor {

	Location from;
	Location to;
	MoveDescriptor() {};
	MoveDescriptor(Location from, Location to) : from(from), to(to) {}
	MoveDescriptor(const MoveDescriptor& pair) { *this = pair; }
	MoveDescriptor& operator=(const MoveDescriptor& pair) {
		if (this == &pair)
			return *this;

		from = pair.from;
		to = pair.to;
		return *this;
	}
};
