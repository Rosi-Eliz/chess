#include "Field.h"

Field::Field()
{
	figure = nullptr;
}

Field::Field(Location location) : location(location) {
	figure = nullptr;
}

Field::Field(const Field& field)
{
	location = field.location; 
	figure = field.figure;
}

Location Field::getLocation() const
{
	return location;
}

void Field::setFigure(Figure* figure)
{
	this->figure = figure;
}

Figure* Field::getFigure() const
{
	return figure;
}