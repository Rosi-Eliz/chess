#pragma once
#include "GraphicsEngine.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct FigureSprite {
	sf::Sprite sprite;
	FigureType figureType;
};

class SFMLGraphicsEngine : public GraphicsEngine
{
	int lastMoveOldPositionRow;
	int lastMoveOldPositionColumn;
	int lastMoveNewPositionRow;
	int lastMoveNewPositionColumn;

	bool shouldRenderEndGameLayout;
	double figureBoxSize;
	void populateFigures(BoardLayout boardLayout);
	void redrawBoard(int indexForZElevation = -1);
	std::vector<FigureSprite> figures;
	std::vector<sf::RectangleShape> possibleMoves;
	sf::Sprite* figureForPosition(int row, int column);
	sf::Sprite* figureForLocation(int x, int y, bool selectCenter = false);
	sf::Vector2f getCoordinates(int row, int column);
	void getLocation(sf::Vector2f coordinates, int& row, int& column);
	void move(int selectedIndex, sf::Vector2f toCoordinates, int animationComplexity = 1000);
	bool removeFigureIgnoringSelection(sf::Vector2f coordinates, int selectedIndex);
	bool removeFigure(sf::Sprite* figureReference);
	void performMoveProcesses(int fromRow, int fromColumn, int toRow, int toColumn, int index, int animationComplexity, bool shouldNotifyReceiver);
	GraphicsEngineProvider* graphicsEngineProvider;
public:
	SFMLGraphicsEngine();
	SFMLGraphicsEngine(GraphicsEngineProvider* graphicsEngineProvider);
	void addFigure(FigureDesignation figure, FigureType figureType, int row, int column);
	void initiateRender(BoardLayout boardLayout = LeadingWhites);
	bool move(int fromRow, int fromColumn, int toRow, int toColumn, bool shouldAnimate = true, bool shouldNotifyReceiver = true);
	bool removeFigure(int row, int column);
	void addPossibleMoveSquare(int row, int column);
	void addLastMoveSquare(int row, int column);
	void removePossibleMoves();
	void removeAllFigures();
	void showGameOverLayout(string message);
	void hideGameOverLayout();
};