#pragma once
#include "Globals.h"
#include "Figure.h"

class Field {
	Location location;
	Figure* figure;
public: 
	Field();
	Field(Location location);
	Field(const Field& field);
	Location getLocation() const;
	void setFigure(Figure* figure);
	Figure* getFigure() const;
};