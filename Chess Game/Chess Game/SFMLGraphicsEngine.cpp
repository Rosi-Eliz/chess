#include <SFML/Graphics.hpp>
#include "SFMLGraphicsEngine.h"
#include <iostream>
#include <math.h>
using namespace std;
using namespace sf;

#define FIGURES_SIZE 32
#define FIGURES_IN_ROW 8
#define ANIMATION_COMPLEXITY 1000
#define FAST_ANIMATION_COMPLEXITY 300

const double boardSize = 900;
const double offsetSize = boardSize * 0.033;
Vector2f offset(offsetSize, offsetSize);
RenderWindow window(VideoMode(boardSize, boardSize), "Chess", Style::Close | Style::Titlebar);

Sprite boardSprite;

int board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

void SFMLGraphicsEngine::initiateRender() {
	Image icon;
	icon.loadFromFile("icons/icon.png");
	window.setIcon(32, 32, icon.getPixelsPtr());

	Texture boardTexture, figuresTexture;
	figuresTexture.loadFromFile("Textures/chessPieces.png");
	figureBoxSize = figuresTexture.getSize().y / 2;

	boardTexture.loadFromFile("Textures/chessBoard.png");

	boardSprite = Sprite(boardTexture);
	boardSprite.setScale(boardSize / boardSprite.getLocalBounds().width, boardSize / boardSprite.getLocalBounds().height);

	for (int i = 0; i < FIGURES_SIZE; i++) {
		Sprite figure;
		figure.setTexture(figuresTexture);
		figures.push_back(figure);
	}

	populateFigures();

	bool isMove = false;
	float dx = 0, dy = 0;
	int selectedFigureIndex = -1;
	Vector2f oldPosition, newPosition;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			Vector2i mousePosition = Mouse::getPosition(window) - Vector2i(offset.x, offset.y);
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (event.key.code == Mouse::Left)
				{
					for (size_t i = 0; i < figures.size(); i++)
					{
						FloatRect figureBounds = figures[i].getGlobalBounds();
						figureBounds.top -= offset.y;
						figureBounds.left -= offset.x;
						if (figureBounds.contains(mousePosition.x, mousePosition.y))
						{
							isMove = true;
							selectedFigureIndex = i;
							dx = mousePosition.x - figures[i].getPosition().x;
							dy = mousePosition.y - figures[i].getPosition().y;
							oldPosition = figures[i].getPosition();
						}
					}
				}
				break;
				
			case Event::MouseButtonReleased:
				if (event.key.code == Mouse::Left)
				{
					isMove = false;
					Vector2f selectedPosition = figures[selectedFigureIndex].getPosition() + Vector2f(figureBoxSize / 2 - offset.x,
						                                                                              figureBoxSize / 2 - offset.y);
					selectedPosition.x = fmin(selectedPosition.x, boardSize - figureBoxSize);
					selectedPosition.y = fmin(selectedPosition.y, boardSize - figureBoxSize);

					newPosition = Vector2f(figureBoxSize * int(selectedPosition.x / figureBoxSize) + offset.x,
										   figureBoxSize * int(selectedPosition.y / figureBoxSize) + offset.y);
					move(selectedFigureIndex, newPosition, FAST_ANIMATION_COMPLEXITY);
					removeFigureIgnoringSelection(newPosition, selectedFigureIndex);
				}

				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Space) {
					//std::cout << "now";
					//removeFigure(1,1);      
					move(1, 1, 3, 3);
				}
				break;
			default:
				break;
			}

			if (isMove) {
				figures[selectedFigureIndex].setPosition(mousePosition.x - dx, mousePosition.y - dy);
			}
		}

		redrawBoard(selectedFigureIndex);
	}
}

void SFMLGraphicsEngine::populateFigures()
{
	int k = 0;
	for (int i = 0; i < FIGURES_IN_ROW; i++)
	{
		for (int j = 0; j < FIGURES_IN_ROW; j++)
		{
			int figurePositionIndex = board[i][j];
			if (!figurePositionIndex)
			{
				continue;
			}

			int x = abs(figurePositionIndex) - 1;
			int y = figurePositionIndex > 0 ? 1 : 0;
			figures[k].setTextureRect(IntRect(figureBoxSize * x, figureBoxSize * y, figureBoxSize, figureBoxSize));
			figures[k].setPosition(figureBoxSize * j + offset.x, figureBoxSize * i + offset.y);
			k++;
		}
	}
}

void SFMLGraphicsEngine::move(int selectedIndex, Vector2f toCoordinates, int animationComplexity) {
	for (int k = 0; k < animationComplexity; k++)
	{
		Vector2f scalar = Vector2f(toCoordinates) - Vector2f(figures[selectedIndex].getPosition());
		figures[selectedIndex].move(scalar.x / animationComplexity, scalar.y / animationComplexity);
		redrawBoard(selectedIndex);
	}

	figures[selectedIndex].setPosition(toCoordinates.x, toCoordinates.y);
}

bool SFMLGraphicsEngine::move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) {
	Vector2f toCoordinates = getCoordinates(toRow, toColumn);
	int animationComplexity = shouldAnimate ? ANIMATION_COMPLEXITY : 0;

	for (size_t i = 0; i < figures.size(); i++)
	{
		Vector2f fromCoordinates = getCoordinates(fromRow, fromColumn);
		// The addition of size/2 is so that we can refer to the field's center
		if (figures[i].getGlobalBounds().contains(fromCoordinates.x + figureBoxSize / 2,
			fromCoordinates.y + figureBoxSize / 2))
		{
			for (int k = 0; k < animationComplexity; k++)
			{
				Vector2f scalar = Vector2f(toCoordinates) - Vector2f(fromCoordinates);
				figures[i].move(scalar.x / animationComplexity, scalar.y / animationComplexity);
				redrawBoard(i);
			}

			figures[i].setPosition(toCoordinates.x, toCoordinates.y);
			return true;
		}
	}
	return false;
}

bool SFMLGraphicsEngine::removeFigureIgnoringSelection(Vector2f coordinates, int selectedIndex) {
	for (size_t i{ 0 }; i < figures.size(); i++) {
		if (i == selectedIndex) {
			continue;
		}
	
		if (figures[i].getGlobalBounds().contains(coordinates.x + figureBoxSize / 2,
			coordinates.y + figureBoxSize / 2)) {
			return removeFigure(&figures[i]);
		}
	}
	return false;
}

bool SFMLGraphicsEngine::removeFigure(Sprite* figureReference) {
	for (size_t i{ 0 }; i < figures.size(); i++) {
		if (&figures[i] == figureReference) {
			figures.erase(figures.begin() + i);
			return true;
		}
	}
	return false;
}

bool SFMLGraphicsEngine::removeFigure(int row, int column) {
	Sprite *figureReference = figureForPosition(row, column);
	return removeFigure(figureReference);
}

Vector2f SFMLGraphicsEngine::getCoordinates(int row, int column) {
	int x = column * figureBoxSize;
	int y = (FIGURES_IN_ROW - 1 - row) * figureBoxSize;
	return Vector2f(x + offset.x, y + offset.y);
}

void SFMLGraphicsEngine::getLocation(sf::Vector2f coordinates, int& row, int& column) {
	column = (coordinates.x - offset.x) / figureBoxSize;
	row = FIGURES_IN_ROW - 1 - floor(coordinates.y / figureBoxSize );
}

Sprite* SFMLGraphicsEngine::figureForPosition(int row, int column) {
	Vector2f coordinates = getCoordinates(row, column);
	return figureForLocation(coordinates.x, coordinates.y, true);
}

Sprite* SFMLGraphicsEngine::figureForLocation(int x, int y, bool selectCenter) {
	float shiftValue = selectCenter ? figureBoxSize / 2 : 0;
	for (size_t i = 0; i < figures.size(); i++)
	{
		if (figures[i].getGlobalBounds().contains(x + shiftValue, y + shiftValue)) {
			Sprite* figureReference = &(figures[i]);
			return figureReference;
		}
	}
	return nullptr;
}

void SFMLGraphicsEngine::redrawBoard(int indexForZElevation) {
	window.clear();
	window.draw(boardSprite);
	for (size_t j = 0; j < figures.size(); j++) {
		window.draw(figures[j]);
	}
	if (indexForZElevation >= 0 && indexForZElevation < figures.size()) {
		window.draw(figures[indexForZElevation]);
	}
	window.display();
}

void SFMLGraphicsEngine::addPossibleMoveSquare(int row, int column) {

}

void SFMLGraphicsEngine::removePossibleMoves() {

}