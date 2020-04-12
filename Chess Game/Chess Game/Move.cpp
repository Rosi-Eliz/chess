#include "Move.h"
#include <string>

Move::Move(Location from, Location to, ChessFigureColor playerColor, ChessFigureDirection playerDirection, Figure* removedFigure)
{
	this->from = from;
	this->to = to;
	this->playerColor = playerColor;
	this->playerDirection = playerDirection;
	this->removedFigure = removedFigure;
	timestamp = time(0);
}

time_t Move::getTime() const
{
	return timestamp;
}

string Move::getDurationSince(time_t startTime) const
{
	int elapsedSeconds = static_cast<int>(difftime(timestamp, startTime));
	int minutes = elapsedSeconds / 60;
	int hours = minutes / 60;
	int seconds = elapsedSeconds % 60;

	string hoursString = hours < 10 ? "0" + to_string(hours) : to_string(hours);
	string minutesString = minutes < 10 ? "0" + to_string(minutes) : to_string(minutes);
	string secondsString = seconds < 10 ? "0" + to_string(seconds) : to_string(seconds);

	return hoursString + " : " + minutesString + " : " + secondsString;
}