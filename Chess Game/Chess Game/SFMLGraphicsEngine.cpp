#include <SFML/Graphics.hpp>
#include "SFMLGraphicsEngine.h"
#include <iostream>
#include <math.h>
#include "List.h"
using namespace std;
using namespace sf;

#define FIGURES_SIZE 32
#define FIGURES_IN_ROW 8
#define ANIMATION_COMPLEXITY 100
#define ANIMATION_DURTATION_SECONDS 0.3

#define FIELD_SELECTION_OUTLINE 0
#define FIELD_SELECTION_COLOR Color(46,114,153,178) //178 alpha corresponding to 0.7 in a [0,1] range
#define FIELD_PREVIOUS_MOVE_COLOR Color(13,163,152,100)
#define DARK_GRAY_COLOR Color(108,108,108,255)
#define FIELD_SELECTION_OUTLINE_COLOR Color(26,116,158)

// 1.8 multiplication coefficient for mac os screens
#ifdef IS_MAC_OS
const double scaleCoefficient = 1.8;
#else
const double scaleCoefficient = 1;
#endif

const double boardSize = 900 * scaleCoefficient;
const double offsetSize = boardSize * 0.033;
Vector2f offset(offsetSize, offsetSize);
RenderWindow window(VideoMode(boardSize, boardSize), "Chess", Style::Close | Style::Titlebar);

Texture boardTexture, figuresTexture, endGameTexture;

Sprite boardSprite;
Sprite endGameSprite;

Font textFont;
Text messageText;

string iconFilePath;
string chessPiecesFilePath;
string textFontFilePath;
string chessBoardFilePath;
string chessBoardInvertedFilePath;
string gameOverLayoutFilePath;


SFMLGraphicsEngine::SFMLGraphicsEngine() {}

SFMLGraphicsEngine::SFMLGraphicsEngine(GraphicsEngineProvider* graphicsEngineProvider): graphicsEngineProvider(graphicsEngineProvider){
    Image icon;
    
#ifdef PROJECT_DIR
    iconFilePath = PROJECT_DIR;
    iconFilePath += "/../../Chess Game/Icons/icon.png";
    
    chessPiecesFilePath = PROJECT_DIR;
    chessPiecesFilePath += "/../../Chess Game/Textures/chessPieces2x.png";
    
    textFontFilePath = PROJECT_DIR;
    textFontFilePath += "/../../Chess Game/Fonts/Candara.ttf";
    
    chessBoardFilePath = PROJECT_DIR;
    chessBoardFilePath += "/../../Chess Game/Textures/chessBoard2x.png";
    
    chessBoardInvertedFilePath = PROJECT_DIR;
    chessBoardInvertedFilePath += "/../../Chess Game/Textures/chessBoardInverted2x.png";
    
    gameOverLayoutFilePath = PROJECT_DIR;
    gameOverLayoutFilePath += "/../../Chess Game/Textures/gameOverLayout.png";
#else
    iconFilePath = "icons/icon.png";
    chessPiecesFilePath = "Textures/chessPieces.png";
    textFontFilePath = "Fonts/Candara.ttf";
    chessBoardFilePath = "Textures/chessBoard.png";
    chessBoardInvertedFilePath = "Textures/chessBoardInverted.png";
    gameOverLayoutFilePath = "Textures/gameOverLayout.png";
#endif
    
    icon.loadFromFile(iconFilePath);
    window.setIcon(32, 32, icon.getPixelsPtr());
    
    figuresTexture.loadFromFile(chessPiecesFilePath);
    figureBoxSize = (boardSize - 2 * offsetSize) / FIGURES_IN_ROW;
}

int board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

void SFMLGraphicsEngine::addFigure(FigureDesignation figure, FigureType figureType, int row, int column) {
	Vector2f coordinates = getCoordinates(row, column);

	int horizontalScale = figure;
	int verticalScale = figureType;

	Sprite sprite;
	sprite.setTexture(figuresTexture);
    double textureSize = figuresTexture.getSize().y / 2;
    sprite.setTextureRect(IntRect(textureSize * horizontalScale, textureSize * verticalScale, textureSize, textureSize));
	sprite.setPosition(coordinates);
    double spriteScale = figureBoxSize / textureSize;
    sprite.setScale(spriteScale, spriteScale);

	FigureSprite figureSprite;
	figureSprite.sprite = sprite;
	figureSprite.figureType = figureType;
	figures.push_back(figureSprite);
}

void SFMLGraphicsEngine::initiateRender(BoardLayout boardLayout) {
	lastMoveOldPositionRow = -1;
	lastMoveOldPositionColumn = -1;
	lastMoveNewPositionRow = -1;
	lastMoveNewPositionColumn = -1;
	shouldRenderEndGameLayout = false;

	textFont.loadFromFile(textFontFilePath);

	messageText.setFont(textFont);
	messageText.setFillColor(DARK_GRAY_COLOR);
	messageText.setCharacterSize(boardSize * 0.024);
	messageText.setPosition(257, 416);

	boardTexture.loadFromFile(boardLayout == LeadingWhites ? chessBoardFilePath : chessBoardInvertedFilePath);
	endGameTexture.loadFromFile(gameOverLayoutFilePath);

	boardSprite = Sprite(boardTexture);
	boardSprite.setScale(boardSize / boardSprite.getLocalBounds().width, boardSize / boardSprite.getLocalBounds().height);

	endGameSprite = Sprite(endGameTexture);
	endGameSprite.setScale(boardSize / endGameSprite.getLocalBounds().width, boardSize / endGameSprite.getLocalBounds().height);


	//populateFigures(boardLayout);

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
					if (isMove || shouldRenderEndGameLayout) {
						continue;
					}
					if (graphicsEngineProvider == nullptr) {
						throw runtime_error("graphicsEngineProvider is not supplied");
					}

					for (size_t i = 0; i < figures.size(); i++)
					{
						FloatRect figureBounds = figures[i].sprite.getGlobalBounds();
						figureBounds.top -= offset.y;
						figureBounds.left -= offset.x;
						if (figureBounds.contains(mousePosition.x, mousePosition.y))
						{
							FigureSprite figure = figures[i];
							if (!graphicsEngineProvider->isPlayerActive(figure.figureType)) {
								continue;
							}

							isMove = true;
							selectedFigureIndex = i;
							Vector2f figurePosition = figure.sprite.getPosition();
							dx = mousePosition.x - figurePosition.x;
							dy = mousePosition.y - figurePosition.y;
							oldPosition = figurePosition;
							int row, column;
							getLocation(figurePosition, row, column);
							List<Location> possibleMoves = graphicsEngineProvider->availableMovesForFigure(row, column);
							for (int i{ 0 }; i < possibleMoves.size(); i++) 
							{
								Location location = possibleMoves[i];
								addPossibleMoveSquare(location.row, location.column);
							}
						}
					}
				}
				break;
				
			case Event::MouseButtonReleased:
				if (event.key.code == Mouse::Left)
				{
					if (selectedFigureIndex == -1) {
						break;
					}
					if (graphicsEngineProvider == nullptr) {
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

					if (!graphicsEngineProvider->isMoveValid(oRow, oColumn, nRow, nColumn)) {
						move(selectedFigureIndex, oldPosition, ANIMATION_COMPLEXITY);
					}
					else {
						performMoveProcesses(oRow, oColumn, nRow, nColumn, selectedFigureIndex, ANIMATION_COMPLEXITY, true);
					}
					selectedFigureIndex = -1;
				}

				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Space) {
					//std::cout << "now";
					//removeFigure(1,1);      
					//move(1, 1, 3, 3);
					//addPossibleMoveSquare(4, 4);
				}
//				if (event.key.code == Keyboard::BackSpace) {
//					graphicsEngineProvider->revertLastMove();
//				}
				if (event.key.code == Keyboard::Enter) {
					if (shouldRenderEndGameLayout)
					{
						hideGameOverLayout();
						graphicsEngineProvider->didRestartGame();
					}

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

void SFMLGraphicsEngine::showGameOverLayout(string message)
{
	if (shouldRenderEndGameLayout)
	{
		return;
	}

	messageText.setString(message);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(Vector2f(boardSize * 0.48, boardSize * 0.5));

	shouldRenderEndGameLayout = true;
	double animationSeconds = ANIMATION_DURTATION_SECONDS;
	double frameSplit = animationSeconds / ANIMATION_COMPLEXITY;

	Clock clockElapsed;
	Clock clockTotal;

	double alphaSplit = double(255) / ANIMATION_COMPLEXITY;
	double currentAlpha = 0;

	while (clockTotal.getElapsedTime().asSeconds() <= animationSeconds) {
		if (clockElapsed.getElapsedTime().asSeconds() >= frameSplit) {
			Clock clock;

			currentAlpha = (1.0 / ( animationSeconds / clockTotal.getElapsedTime().asSeconds())) * 255;
			endGameSprite.setColor(Color(255, 255, 255, currentAlpha));
			messageText.setFillColor(Color(108, 108, 108, currentAlpha));
			redrawBoard();
			currentAlpha += alphaSplit;
			clock.restart();
			clockElapsed.restart();
		}
	}
	endGameSprite.setColor(Color(255, 255, 255, 255));
	messageText.setFillColor(Color(108, 108, 108, 255));
}

void SFMLGraphicsEngine::hideGameOverLayout()
{
	if (!shouldRenderEndGameLayout)
	{
		return;
	}
	double animationSeconds = ANIMATION_DURTATION_SECONDS;
	double frameSplit = animationSeconds / ANIMATION_COMPLEXITY;

	Clock clockElapsed;
	Clock clockTotal;

	double alphaSplit = double(255) / ANIMATION_COMPLEXITY;
	double currentAlpha = 0;

	while (clockTotal.getElapsedTime().asSeconds() <= animationSeconds) {
		if (clockElapsed.getElapsedTime().asSeconds() >= frameSplit) {
			Clock clock;

			currentAlpha = (1.0 / (animationSeconds / clockTotal.getElapsedTime().asSeconds())) * 255;
			endGameSprite.setColor(Color(255, 255, 255, 255 - currentAlpha));
			messageText.setFillColor(Color(108, 108, 108, 255 - currentAlpha));
			redrawBoard();
			currentAlpha += alphaSplit;
			clock.restart();
			clockElapsed.restart();
		}
	}
	shouldRenderEndGameLayout = false;
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
		addFigure(PawnDesignation, leadingFigureType, leadingPlayerPawnRow, i);
	}

	//Leading other figures
	addFigure(RookDesignation, leadingFigureType, leadingPlayerOthersRow, 0);
	addFigure(KnightDesignation, leadingFigureType, leadingPlayerOthersRow, 1);
	addFigure(BishopDesignation, leadingFigureType, leadingPlayerOthersRow, 2);
	addFigure(QueenDesignation, leadingFigureType, leadingPlayerOthersRow, 3);
	addFigure(KingDesignation, leadingFigureType, leadingPlayerOthersRow, 4);
	addFigure(BishopDesignation, leadingFigureType, leadingPlayerOthersRow, 5);
	addFigure(KnightDesignation, leadingFigureType, leadingPlayerOthersRow, 6);
	addFigure(RookDesignation, leadingFigureType, leadingPlayerOthersRow, 7);

	//Trailing figures pawns
	for (int i{ 0 }; i < FIGURES_IN_ROW; i++) {
		addFigure(PawnDesignation, trailingFigureType, trailingPlayerPawnRow, i);
	}

	//Trailing other figures
	addFigure(RookDesignation, trailingFigureType, trailingPlayerOthersRow, 0);
	addFigure(KnightDesignation, trailingFigureType, trailingPlayerOthersRow, 1);
	addFigure(BishopDesignation, trailingFigureType, trailingPlayerOthersRow, 2);
	addFigure(QueenDesignation, trailingFigureType, trailingPlayerOthersRow, 3);
	addFigure(KingDesignation, trailingFigureType, trailingPlayerOthersRow, 4);
	addFigure(BishopDesignation, trailingFigureType, trailingPlayerOthersRow, 5);
	addFigure(KnightDesignation, trailingFigureType, trailingPlayerOthersRow, 6);
	addFigure(RookDesignation, trailingFigureType, trailingPlayerOthersRow, 7);
}

void SFMLGraphicsEngine::move(int selectedIndex, Vector2f toCoordinates, int animationComplexity) {
	if (animationComplexity == 0) {
		figures[selectedIndex].sprite.setPosition(toCoordinates.x, toCoordinates.y);
		return;
	}
	Vector2f selectedFigureCoordinates = figures[selectedIndex].sprite.getPosition();

	float horizontalDistance = toCoordinates.x - selectedFigureCoordinates.x;
	float verticalDistance = toCoordinates.y - selectedFigureCoordinates.y;
	float distance = sqrtf(powf(horizontalDistance, 2) + powf(verticalDistance, 2));
	Vector2f connectingVector(horizontalDistance, verticalDistance);

	float xRelativeMove = connectingVector.x / animationComplexity;
	float yRelativeMove = connectingVector.y / animationComplexity;

	Clock clockElapsed;
	Clock clockTotal;

	double animationSeconds = ANIMATION_DURTATION_SECONDS;
	double frameSplit = animationSeconds / animationComplexity;

	if (distance < 100) {
		animationSeconds /= 3;
		frameSplit /= 3;
	}

	while (clockTotal.getElapsedTime().asSeconds() <= animationSeconds) {
		if (clockElapsed.getElapsedTime().asSeconds() >= frameSplit) {
			Clock clock;
			figures[selectedIndex].sprite.move(xRelativeMove, yRelativeMove);
			redrawBoard(selectedIndex);
			animationSeconds += clock.getElapsedTime().asSeconds();
			clock.restart();
			clockElapsed.restart();
		}
	}


	figures[selectedIndex].sprite.setPosition(toCoordinates.x, toCoordinates.y);
}

bool SFMLGraphicsEngine::move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate, bool shouldNotifyReceiver) {
	int animationComplexity = shouldAnimate ? ANIMATION_COMPLEXITY : 0;

	for (size_t i = 0; i < figures.size(); i++)
	{
		Vector2f fromCoordinates = getCoordinates(fromRow, fromColumn);
		// The addition of size/2 is so that we can refer to the field's center
		if (figures[i].sprite.getGlobalBounds().contains(fromCoordinates.x + figureBoxSize / 2,
			fromCoordinates.y + figureBoxSize / 2))
		{
			performMoveProcesses(fromRow, fromColumn, toRow, toColumn, i, animationComplexity, shouldNotifyReceiver);
			return true;
		}
	}
	return false;
}

void SFMLGraphicsEngine::performMoveProcesses(int fromRow, int fromColumn, int toRow, int toColumn, int index, int animationComplexity, bool shouldNotifyReceiver)
{
	Vector2f toCoordinates = getCoordinates(toRow, toColumn);

	lastMoveOldPositionRow = fromRow;
	lastMoveOldPositionColumn = fromColumn;
	lastMoveNewPositionRow = toRow;
	lastMoveNewPositionColumn = toColumn;

	move(index, toCoordinates, animationComplexity);
	removeFigureIgnoringSelection(toCoordinates, index);

	if (shouldNotifyReceiver)
	{
		graphicsEngineProvider->didMove(fromRow, fromColumn, toRow, toColumn);
	}
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
			if (graphicsEngineProvider == nullptr) {
				throw runtime_error("didRemoveFigure is not supplied");
			}
			graphicsEngineProvider->didRemoveFigure(row, column);
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

void SFMLGraphicsEngine::removeAllFigures()
{
	lastMoveOldPositionRow = -1;
	lastMoveOldPositionColumn = -1;
	lastMoveNewPositionRow = -1;
	lastMoveNewPositionColumn = -1;
	figures = vector<FigureSprite>();
}
Vector2f SFMLGraphicsEngine::getCoordinates(int row, int column) {
    int x = column * figureBoxSize;
    int y = (FIGURES_IN_ROW - 1 - row) * figureBoxSize;
	return Vector2f(x + offset.x, y + offset.y);
}

void SFMLGraphicsEngine::getLocation(sf::Vector2f coordinates, int& row, int& column) {
	column = (coordinates.x - offset.x + figureBoxSize/2) / figureBoxSize;
	row = FIGURES_IN_ROW - 1 - floor((coordinates.y - offset.y + figureBoxSize/2) / figureBoxSize);
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

	if (lastMoveOldPositionRow != -1 &&
		lastMoveOldPositionColumn != -1 &&
		lastMoveNewPositionRow != -1 &&
		lastMoveNewPositionColumn != -1)
	{
		addLastMoveSquare(lastMoveOldPositionRow, lastMoveOldPositionColumn);
		addLastMoveSquare(lastMoveNewPositionRow, lastMoveNewPositionColumn);
	}

	for (size_t i{ 0 }; i < possibleMoves.size(); i++) {
		window.draw(possibleMoves[i]);
	}
	for (size_t j{ 0 }; j < figures.size(); j++) {
		window.draw(figures[j].sprite);
	}
	if (indexForZElevation >= 0 && indexForZElevation < figures.size()) {
		window.draw(figures[indexForZElevation].sprite);
	}

	if (shouldRenderEndGameLayout)
	{
		window.draw(endGameSprite);
		window.draw(messageText);
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

void SFMLGraphicsEngine::addLastMoveSquare(int row, int column) {
	Vector2f coordinates = getCoordinates(row, column);
	coordinates.x += FIELD_SELECTION_OUTLINE;
	coordinates.y += FIELD_SELECTION_OUTLINE;

	RectangleShape rectangle(coordinates);
	rectangle.setPosition(coordinates);

	rectangle.setFillColor(FIELD_PREVIOUS_MOVE_COLOR);

	rectangle.setOutlineThickness(FIELD_SELECTION_OUTLINE);
	rectangle.setOutlineColor(FIELD_PREVIOUS_MOVE_COLOR);

	rectangle.setSize(Vector2f(figureBoxSize - 2 * FIELD_SELECTION_OUTLINE,
		figureBoxSize - 2 * FIELD_SELECTION_OUTLINE));
	window.draw(rectangle);
}

void SFMLGraphicsEngine::removePossibleMoves() {
	possibleMoves =		{};
}
