#include <SFML/Graphics.hpp>
#include "SFMLGraphicsEngine.h"
using namespace sf;

#define FIGURES_SIZE 32
#define FIGURES_IN_ROW 8

const double boardSize = 900;
const double offsetSize = boardSize * 0.033;
Vector2f offset(offsetSize, offsetSize);

Sprite figures[FIGURES_SIZE];

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
	RenderWindow window(VideoMode(boardSize, boardSize), "Chess", Style::Close | Style::Titlebar);

	Image icon;
	icon.loadFromFile("icons/icon.png");
	window.setIcon(32, 32, icon.getPixelsPtr());

	Texture boardTexture, figuresTexture;
	figuresTexture.loadFromFile("Textures/chessPieces.png");
	figureBoxSize = figuresTexture.getSize().y / 2;

	boardTexture.loadFromFile("Textures/chessBoard.png");

	Sprite boardSprite(boardTexture);
	boardSprite.setScale(boardSize / boardSprite.getLocalBounds().width, boardSize / boardSprite.getLocalBounds().height);

	for (int i = 0; i < FIGURES_SIZE; i++)
	{
		figures[i].setTexture(figuresTexture);
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
					for (int i = 0; i < FIGURES_SIZE; i++)
					{
						if (figures[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
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
					Vector2f selectedPosition = figures[selectedFigureIndex].getPosition() + Vector2f(figureBoxSize / 2, figureBoxSize / 2);
					newPosition = Vector2f(figureBoxSize * int(selectedPosition.x / figureBoxSize), figureBoxSize * int(selectedPosition.y / figureBoxSize));
					figures[selectedFigureIndex].setPosition(newPosition);
				}

				break;
			default:
				break;
			}

			if (isMove) {
				figures[selectedFigureIndex].setPosition(mousePosition.x - dx, mousePosition.y - dy);
			}
		}

		window.clear();
		window.draw(boardSprite);
		
		for (int i = 0; i < FIGURES_SIZE; i++) {
			figures[i].move(offset);
		};
		for (int i = 0; i < FIGURES_SIZE; i++) {
			window.draw(figures[i]);
		};
		if (selectedFigureIndex >= 0) {
			window.draw(figures[selectedFigureIndex]);
		}
		for (int i = 0; i < FIGURES_SIZE; i++) {
			figures[i].move(-offset);
		};
		window.display();
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
			figures[k].setPosition(figureBoxSize * j, figureBoxSize * i);
			k++;
		}
	}
}