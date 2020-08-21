# Chess

An end-to-end chess game providing a configurable artificial intelligence as an opponent.

## Overview

The project is operatable on both Mac OS and Windows, using the `Xcode` and `Visual Studio` IDEs respectively. The artificial intelligence is implemented using the minimax algorithm, optimized by the alpha-beta pruning algorithm. The heuristics for evaluating the board is described below. The project uses the [SFML Framework](https://www.sfml-dev.org/) for rendering the visual elements.

#### Configuring the artificial intelligence:

The artificial intelligence, represented by the `AIEngine` class, has 4 distinct difficulties as defined by the `Difficulty` enum. They are differing in the number of moves that the AI is analysing (the recursive depth, which will be reached from the initial minimax invocation).
##### Difficulties:
- Beginner - 2 moves
- Intermediate - 3 moves
- Advanced - 4 moves
- Expert - 5 moves

In order to change the difficulty navigate to the `Game.cpp` source file and in the designated constructor modify the `Difficulty::Beginner` entry from the last line:
```C++
Game::Game(ChessBoardLayout layout)
{
	graphicsEngine = SFMLGraphicsEngine(this);
	board = new Board(layout, this);
	figuresTurn = ChessFigureColor::White;
	startTime = time(0);
	artificialIntelligenceEngine = AIEngine(Difficulty::Beginner, ChessFigureColor::Black, this);
}
```
#### 1v1
The game can be configured for 1v1 mode. This can be done by navigating to the `Game.cpp` source file and commenting out the following lines from the `void Game::didMove(int fromRow, int fromColumn, int toRow, int toColumn)` method:
```C++
if (figuresTurn == artificialIntelligenceEngine.initialColor)
{
	artificialIntelligenceEngine.findBestMoveIn(*board);
}
```

#### Heuristics
Each chess piece is assigned with a value, corresponding to its type. Upon evaluating the board all pieces' values are multiply by a coefficient, correlating to their location in the board (Generally it is considered more favourable to have control over the centre of the board). All pieces have different position coefficients. They can be observed in the classes derived by `Figure` as a matrix declared from the static variable `valueAdditives`. The multiply values are summed up for the current player. Then this sum is subtracted from the opponent's sum. This would encourage the AI to take pieces from the opponent, while not losing its own. Additionally, the AI is encouraged to have protection over its figures. Furthermore, the AI is rewarded for performing castling moves when possible and declaring checks.

## Demonstration

![](demo.gif)
