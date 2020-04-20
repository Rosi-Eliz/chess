#include "Game.h"
#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "Figure.h"
#include "SFMLGraphicsEngine.h" 
#include <functional>


Game::Game() {
}

Game::Game(ChessBoardLayout layout)
{
	graphicsEngine = SFMLGraphicsEngine(this);
	board = new Board(layout, this);
	figuresTurn = ChessFigureColor::White;
	startTime = time(0);
	artificialIntelligenceEngine = AIEngine(Difficulty::Beginner, ChessFigureColor::Black, this);
}

Game& Game::operator=(const Game& game)
{
	if (this == &game)
	{
		return *this;
	}

	for (int i{ 0 }; i < completedMoves.size(); i++)
	{
		delete completedMoves[i];
	}
	delete board;

	*this = Game(game);
	return *this;
}

Game::Game(const Game& game)
{
	if (this == &game)
		return;
	graphicsEngine = SFMLGraphicsEngine(this);
	figuresTurn = game.figuresTurn;

	for (int i{ 0 }; i < game.completedMoves.size(); i++)
	{
		Move* newMove = game.completedMoves[i];
		completedMoves.pushFront(newMove);
	}

	board = new Board(*game.board);
}

Game::~Game()
{
	for (int i{ 0 }; i < completedMoves.size(); i++)
	{
		delete completedMoves[i];
	}
	delete board;
}

State Game::checkState() 
{
	List<Figure*> currentPlayerPossibleMoves = board->remainingFigures(figuresTurn);
	for (int i{ 0 }; i < currentPlayerPossibleMoves.size(); i++)
	{
		Figure* currentFigure = currentPlayerPossibleMoves[i];
		Field* field = board->getField(currentFigure);
		Location location = field->getLocation();
		if (availableMovesForFigure(location.row, location.column).size() > 0)
		{
			return State::InProgress;
		}
	}
	ChessFigureColor opponentColor = returnOpponentColor(figuresTurn);
	bool conflict = board->fieldIsInConflict(opponentColor);
	if (conflict)
	{
		switch (opponentColor)
		{
		case ChessFigureColor::Black: 
			return State::BlackWins; 
		case ChessFigureColor::White:
			return State::WhiteWins;
		}
	}
	return State::Draw;

}

void Game::switchTurn()
{
	int currentTurn = static_cast<int>(figuresTurn);
	figuresTurn = static_cast<ChessFigureColor>((currentTurn + 1) % 2);
}

void Game::initiateGame()
{
	BoardLayout boardLayout = board->getLayout() == ChessBoardLayout::LeadingBlacks ?
		BoardLayout::LeadingBlacks : BoardLayout::LeadingWhites;

	populateFigures();

	/*
	List<Figure*> blackFigures = board->remainingFigures(ChessFigureColor::Black);

	auto print = [](Figure* figure) {
		cout << figure->getName() << " " << figure->getColorString() << endl;
	};
	blackFigures.forEach(print);
	*/

	graphicsEngine.initiateRender(boardLayout);
}

void Game::restart(ChessBoardLayout layout)
{
	for (int i{ 0 }; i < completedMoves.size(); i++)
	{
		delete completedMoves[i];
	}
	completedMoves = List<Move*>();
	board = new Board(layout, this);
	figuresTurn = ChessFigureColor::White;
	graphicsEngine.removeAllFigures();
	startTime = time(0);
	initiateGame();
}

void Game::populateFigures()
{
	List<Field*> fields = board->getFields(); 
	for (int i{ 0 }; i < fields.size(); i++)
	{
		Field* currentField = fields[i];
		if (currentField == nullptr || currentField->getFigure() == nullptr) 
		{
			continue;
		}
		Figure* figure = currentField->getFigure();

		Location currentLocation = currentField->getLocation();
		int row = currentLocation.row;
		int col = currentLocation.column;
		renderNewFigure(figure, row, col);
	}
}

void Game::renderNewFigure(Figure* figure, int row, int col) {
	FigureType figureType = figure->getColor() == ChessFigureColor::Black ? Black : White;
	if (typeid(*figure) == typeid(Pawn))
	{
		graphicsEngine.addFigure(PawnDesignation, figureType, row, col);
	}
	else if (typeid(*figure) == typeid(King))
	{
		graphicsEngine.addFigure(KingDesignation, figureType, row, col);
	}
	else if (typeid(*figure) == typeid(Queen))
	{
		graphicsEngine.addFigure(QueenDesignation, figureType, row, col);
	}
	else if (typeid(*figure) == typeid(Rook))
	{
		graphicsEngine.addFigure(RookDesignation, figureType, row, col);
	}
	else if (typeid(*figure) == typeid(Knight))
	{
		graphicsEngine.addFigure(KnightDesignation, figureType, row, col);
	}
	else if (typeid(*figure) == typeid(Bishop))
	{
		graphicsEngine.addFigure(BishopDesignation, figureType, row, col);
	}
}

void Game::didRemoveFigure(int row, int column) {


	cout << "removed: row: " << row << ", column: " << column << endl;
}

bool Game::isMoveValid(int fromRow, int fromColumn, int toRow, int toColumn) {
	List<Location> moves = availableMovesForFigure(fromRow, fromColumn);
	return moves.contains([toRow, toColumn](Location location) {
		return location.column == toColumn && location.row == toRow;
	});
}

void Game::createMoveLog(Location from, Location to)
{
	Field* fromField = board->getFieldAt(from);
	Field* toField = board->getFieldAt(from);
	if (fromField == nullptr || toField == nullptr)
		return;
	Figure* figureMoved = fromField->getFigure();
	if (figureMoved == nullptr)
		return;
	Move* newMove = new Move(from, to, figureMoved->getColor(), figureMoved->getDirection(), toField->getFigure());
	completedMoves.pushFront(newMove);
	//std::cout << newMove->getDurationSince(startTime)<<endl;

 }

void Game::didMove(int fromRow, int fromColumn, int toRow, int toColumn) {
	cout << "VALUE IS: " << artificialIntelligenceEngine.evaluateBoard(board, figuresTurn, true);
	switchTurn();

	Location oldLocation = Location(fromRow, fromColumn);
	Location newLocation = Location(toRow, toColumn);
	createMoveLog(oldLocation, newLocation);
	//if(typeid(movedFigure) == typeid(Rook))
	board->updateMove(oldLocation, newLocation);
	
	State currentState = checkState();
	if (currentState != State::InProgress)
	{
		string gameMessage;
		switch (currentState)
		{
		case State::WhiteWins : 
			gameMessage = "Victory for the white player";
			break;
		case State::BlackWins :
			gameMessage = "Victory for the black player";
			break;
		case State::Draw :
			gameMessage = "Tie";
			break;
		}

		string movesMessage = "Completed moves: " + to_string(completedMoves.size());
		string duration = "Total game duration: " + completedMoves[completedMoves.size() - 1]->getDurationSince(startTime);

		string result = gameMessage + "\n" + movesMessage + "\n" + duration;
		graphicsEngine.showGameOverLayout(result);

	}

	//TODO: Remove hardcoded logic 
	if (figuresTurn == ChessFigureColor::Black)
	{
		//artificialIntelligenceEngine.findBestMoveIn(*board);
	}

	cout << "state: " << static_cast<int>(checkState()) << endl;
	cout << "move from - row: " << fromRow << ", column: " << fromColumn << ". To - row: ";
	cout << toRow << ", column: " << toColumn << endl;

}

bool Game::isPlayerActive(FigureType figureType) {
	bool whiteColorMathcingCondition = figureType == White && figuresTurn == ChessFigureColor::White;
	bool blackColorMathcingCondition = figureType == Black && figuresTurn == ChessFigureColor::Black;
	return whiteColorMathcingCondition || blackColorMathcingCondition;
}

List<Location> Game::availableMovesForFigure(int row, int column) {
	return board->availableMovesForFigure(row, column);
}

void Game::removeFigureAt(int row, int col)
{
	graphicsEngine.removeFigure(row, col);
}
void Game::addFigureAt(int row, int col)
{
	Location location(row, col);
	Figure* figure = board->figureAt(location);
	renderNewFigure(figure, row, col);
}

void Game::didRestartGame()
{
	restart(board->getLayout());
}

void Game::didChooseBestMove(int fromRow, int fromCol, int toRow, int toCol)
{
	graphicsEngine.move(fromRow, fromCol, toRow, toCol);
}

void Game::move(int fromRow, int fromCol, int toRow, int toCol, bool shouldNotifyReceiver)
{
	graphicsEngine.move(fromRow, fromCol, toRow, toCol, true, shouldNotifyReceiver);  
}

void Game::revertLastMove()
{
	board->revertLastMove();
}