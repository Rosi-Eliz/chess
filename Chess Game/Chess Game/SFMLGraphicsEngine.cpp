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

#define FIELD_SELECTION_OUTLINE 0
#define FIELD_SELECTION_COLOR Color(46,114,153,178) //178 alpha corresponding to 0.7 in a [0,1] range
#define FIELD_SELECTION_OUTLINE_COLOR Color(26,116,158)

const double boardSize = 900;
const double offsetSize = boardSize * 0.033;
Vector2f offset(offsetSize, offsetSize);
RenderWindow window(VideoMode(boardSize, boardSize), "Chess", Style::Close | Style::Titlebar);

Texture boardTexture, figuresTexture;

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

void SFMLGraphicsEngine::addFigure(Figure figure, FigureType figureType, int row, int column) {
	Vector2f coordinates = getCoordinates(row, column);

	int horizontalScale = figure;
	int verticalScale = figureType;

	Sprite sprite;
	sprite.setTexture(figuresTexture);
	sprite.setTextureRect(IntRect(figureBoxSize * horizontalScale, figureBoxSize * verticalScale, figureBoxSize, figureBoxSize));
	sprite.setPosition(coordinates);

	FigureSprite figureSprite;
	figureSprite.sprite = sprite;
	figureSprite.figureType = figureType;
	figures.push_back(figureSprite);
}

void SFMLGraphicsEngine::initiateRender(BoardLayout boardLayout) {
	Image icon;
	icon.loadFromFile("icons/icon.png");
	window.setIcon(32, 32, icon.getPixelsPtr());

	figuresTexture.loadFromFile("Textures/chessPieces.png");
	figureBoxSize = figuresTexture.getSize().y / 2;

	boardTexture.loadFromFile(boardLayout == LeadingWhites ? "Textures/chessBoard.png" : "Textures/chessBoardInverted.png");

	boardSprite = Sprite(boardTexture);
	boardSprite.setScale(boardSize / boardSprite.getLocalBounds().width, boardSize / boardSprite.getLocalBounds().height);

	populateFigures(boardLayout);

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
					if (availableMovesForFigure == nullptr) {
						throw runtime_error("availableMovesForFigure is not supplied");
					}

					for (size_t i = 0; i < figures.size(); i++)
					{
						FloatRect figureBounds = figures[i].sprite.getGlobalBounds();
						figureBounds.top -= offset.y;
						figureBounds.left -= offset.x;
						if (figureBounds.contains(mousePosition.x, mousePosition.y))
						{
							isMove = true;
							selectedFigureIndex = i;
							Vector2f figurePosition = figures[i].sprite.getPosition();
							dx = mousePosition.x - figurePosition.x;
							dy = mousePosition.y - figurePosition.y;
							oldPosition = figurePosition;

							int row, column;
							getLocation(figurePosition, row, column);
							vector<Location> possibleMoves = availableMovesForFigure(row, column);
							for (Location location : possibleMoves) {
								addPossibleMoveSquare(location.row, location.column);
							}
						}
					}
				}
				break;
				
			case Event::MouseButtonReleased:
				if (event.key.code == Mouse::Left)
				{
					if (isMoveValid == nullptr || didMove == nullptr) {
						throw runtime_error("isMoveValid or didMove are not supplied");
					}

					removePossibleMoves();
					isMove = false;
					Vector2f selectedPosition = figures[selectedFigureIndex].sprite.getPosition() + Vector2f(figureBoxSize / 2 - offset.x,
						                                                                              figureBoxSize / 2 - offset.y);
					selectedPosition.x = fmin(selectedPosition.x, boardSize - figureBoxSize);
					selectedPosition.y = fmin(selectedPosition.y, boardSize - figureBoxSize);

					newPosition = Vector2f(figureBoxSize * int(selectedPosition.x / figureBoxSize) + offset.x,
										   figureBoxSize * int(selectedPosition.y / figureBoxSize) + offset.y);

					int oRow, oColumn, nRow, nColumn;
					getLocation(oldPosition, oRow, oColumn);
					getLocation(newPosition, nRow, nColumn);

					if (!isMoveValid(oRow, oColumn, nRow, nColumn)) {
						move(selectedFigureIndex, oldPosition, ANIMATION_COMPLEXITY);
					}
					else {
						move(selectedFigureIndex, newPosition, FAST_ANIMATION_COMPLEXITY);
						removeFigureIgnoringSelection(newPosition, selectedFigureIndex);
						didMove(oRow, oColumn, nRow, nColumn);
					}
				}

				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Space) {
					//std::cout << "now";
					//removeFigure(1,1);      
					//move(1, 1, 3, 3);
					//addPossibleMoveSquare(4, 4);
				}
				if (event.key.code == Keyboard::T) {
					//addPossibleMoveSquare(4, 5);
				}
				if (event.key.code == Keyboard::BackSpace) {
					//removePossibleMoves();
				}
				break;
			default:
				break;
			}

			if (isMove) {
				figures[selectedFigureIndex].sprite.setPosition(mousePosition.x - dx, mousePosition.y - dy);
			}
		}

		redrawBoard(selectedFigureIndex);
	}
}

void SFMLGraphicsEngine::populateFigures(BoardLayout boardLayout)
{
	int leadingPlayerPawnRow = boardLayout == LeadingWhites ? 1 : 6;
	int leadingPlayerOthersRow = boardLayout == LeadingWhites ? 0 : 7;

	int trailingPlayerPawnRow = boardLayout == LeadingWhites ? 6 : 1;
	int trailingPlayerOthersRow = boardLayout == LeadingWhites ? 7 : 0;

	FigureType leadingFigureType = White;
	FigureType trailingFigureType = Black;

	//Leading figures pawns
	for (int i{ 0 }; i < FIGURES_IN_ROW; i++) {
		addFigure(Pawn, leadingFigureType, leadingPlayerPawnRow, i);
	}

	//Leading other figures
	addFigure(Rook, leadingFigureType, leadingPlayerOthersRow, 0);
	addFigure(Knight, leadingFigureType, leadingPlayerOthersRow, 1);
	addFigure(Bishop, leadingFigureType, leadingPlayerOthersRow, 2);
	addFigure(Queen, leadingFigureType, leadingPlayerOthersRow, 3);
	addFigure(King, leadingFigureType, leadingPlayerOthersRow, 4);
	addFigure(Bishop, leadingFigureType, leadingPlayerOthersRow, 5);
	addFigure(Knight, leadingFigureType, leadingPlayerOthersRow, 6);
	addFigure(Rook, leadingFigureType, leadingPlayerOthersRow, 7);

	//Trailing figures pawns
	for (int i{ 0 }; i < FIGURES_IN_ROW; i++) {
		addFigure(Pawn, trailingFigureType, trailingPlayerPawnRow, i);
	}

	//Trailing other figures
	addFigure(Rook, trailingFigureType, trailingPlayerOthersRow, 0);
	addFigure(Knight, trailingFigureType, trailingPlayerOthersRow, 1);
	addFigure(Bishop, trailingFigureType, trailingPlayerOthersRow, 2);
	addFigure(Queen, trailingFigureType, trailingPlayerOthersRow, 3);
	addFigure(King, trailingFigureType, trailingPlayerOthersRow, 4);
	addFigure(Bishop, trailingFigureType, trailingPlayerOthersRow, 5);
	addFigure(Knight, trailingFigureType, trailingPlayerOthersRow, 6);
	addFigure(Rook, trailingFigureType, trailingPlayerOthersRow, 7);
}

void SFMLGraphicsEngine::move(int selectedIndex, Vector2f toCoordinates, int animationComplexity) {
	Vector2f selectedFigureCoordinates = figures[selectedIndex].sprite.getPosition();

	float horizontalDistance = toCoordinates.x - selectedFigureCoordinates.x;
	float verticalDistance = toCoordinates.y - selectedFigureCoordinates.y;
	float distance = sqrtf(powf(horizontalDistance, 2) + powf(verticalDistance, 2));
	Vector2f connectingVector(horizontalDistance, verticalDistance);

	float xRelativeMove = connectingVector.x / animationComplexity;
	float yRelativeMove = connectingVector.y / animationComplexity;

	for (int k = 0; k < animationComplexity; k++)
	{	
		figures[selectedIndex].sprite.move(xRelativeMove, yRelativeMove);
		redrawBoard(selectedIndex);
	}

	figures[selectedIndex].sprite.setPosition(toCoordinates.x, toCoordinates.y);
}

bool SFMLGraphicsEngine::move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate) {
	Vector2f toCoordinates = getCoordinates(toRow, toColumn);
	int animationComplexity = shouldAnimate ? ANIMATION_COMPLEXITY : 0;

	for (size_t i = 0; i < figures.size(); i++)
	{
		Vector2f fromCoordinates = getCoordinates(fromRow, fromColumn);
		// The addition of size/2 is so that we can refer to the field's center
		if (figures[i].sprite.getGlobalBounds().contains(fromCoordinates.x + figureBoxSize / 2,
			fromCoordinates.y + figureBoxSize / 2))
		{
			for (int k = 0; k < animationComplexity; k++)
			{
				Vector2f scalar = Vector2f(toCoordinates) - Vector2f(fromCoordinates);
				figures[i].sprite.move(scalar.x / animationComplexity, scalar.y / animationComplexity);
				redrawBoard(i);
			}

			figures[i].sprite.setPosition(toCoordinates.x, toCoordinates.y);
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
	
		if (figures[i].sprite.getGlobalBounds().contains(coordinates.x + figureBoxSize / 2,
			coordinates.y + figureBoxSize / 2)) {
			int row, column;
			getLocation(coordinates, row, column);
			if (didRemoveFigure == nullptr) {
				throw runtime_error("didRemoveFigure is not supplied");
			}
			didRemoveFigure(row, column);
			return removeFigure(&figures[i].sprite);
		}
	}
	return false;
}

bool SFMLGraphicsEngine::removeFigure(Sprite* figureReference) {
	for (size_t i{ 0 }; i < figures.size(); i++) {
		if (&figures[i].sprite == figureReference) {
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
		if (figures[i].sprite.getGlobalBounds().contains(x + shiftValue, y + shiftValue)) {
			Sprite* figureReference = &(figures[i].sprite);
			return figureReference;
		}
	}
	return nullptr;
}

void SFMLGraphicsEngine::redrawBoard(int indexForZElevation) {
	window.clear();
	window.draw(boardSprite);
	for (size_t i{ 0 }; i < possibleMoves.size(); i++) {
		window.draw(possibleMoves[i]);
	}
	for (size_t j{ 0 }; j < figures.size(); j++) {
		window.draw(figures[j].sprite);
	}
	if (indexForZElevation >= 0 && indexForZElevation < figures.size()) {
		window.draw(figures[indexForZElevation].sprite);
	}
	window.display();
}

void SFMLGraphicsEngine::addPossibleMoveSquare(int row, int column) {
	Vector2f coordinates = getCoordinates(row, column);
	coordinates.x += FIELD_SELECTION_OUTLINE;
	coordinates.y += FIELD_SELECTION_OUTLINE;

	RectangleShape rectangle(coordinates);
	rectangle.setPosition(coordinates);

	rectangle.setFillColor(FIELD_SELECTION_COLOR);

	rectangle.setOutlineThickness(FIELD_SELECTION_OUTLINE);
	rectangle.setOutlineColor(FIELD_SELECTION_OUTLINE_COLOR);
	
	rectangle.setSize(Vector2f(figureBoxSize - 2 * FIELD_SELECTION_OUTLINE,
							   figureBoxSize - 2 * FIELD_SELECTION_OUTLINE));
	possibleMoves.push_back(rectangle);
}

void SFMLGraphicsEngine::removePossibleMoves() {
	possibleMoves = {};
}