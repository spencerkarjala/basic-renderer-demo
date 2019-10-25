#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "Fruit.h"
#include "Color.h"
#include "Dimension.h"
#include "Tetrominoes/TetrominoI.h"
#include "Tetrominoes/TetrominoL.h"
#include "Tetrominoes/TetrominoS.h"
#include "Tetrominoes/TetrominoT.h"

/*  Contains the logic that manages any board pieces like placed fruits, the
    current tetromino in play, dimensions for bounds and collision handling,
    and dimensions for rendering the game board.                                */

class Board {

    public:
    Board();
    Board(int width, int height, int numCols, int numRows, int xPosition, int yPosition);
    ~Board();

    Dimension getBoardSize();
    void setBoardSize(Dimension newBoardSize);

    Dimension getCellSize();
    void setCellSize(Dimension newCellSize);

    int getNumCols();
    void setNumCols(const int numCols);

    int getNumRows();
    void setNumRows(const int numRows);

    Coordinate getPosition();
    void setPosition(Coordinate newCoordinate);
    void setPosition(int newPositionX, int newPositionY);

    Color getGridColor();
    void setGridColor(Color newGridColor); 

    Tetromino* getTetromino();
    void setTetromino(Tetromino* tetromino);

    Fruit*** getFruits();
    void registerFruit(Fruit* newFruit);
    void removeFruit(Fruit* fruitToRemove);

    bool tetrominoIsOnBoard();
    bool isOutOfBoundsAt(Coordinate fruitPos);
    bool isCollisionAt(Coordinate fruitPos);
    bool isEmptyAt(Coordinate fruitPos);

    void setCellsToGray();

    std::vector<int> findFullRows();
    void clearRow(int row);
    void clearFruitCombos();
    void clearFruitCombosAt(Coordinate position);
    int findComboLength(Coordinate position, Vector direction, int depth);
    int findMaxFallDistance(Tetromino* tetromino); 
    int findMaxFallingDistance(Fruit* fruit, Tetromino* tetromino);

    private:
    Dimension boardSize;
    Dimension cellSize;
    int numCols;
    int numRows;
    Coordinate position;
    Color gridColor;
    Tetromino* tetromino;
    Fruit*** fruits;

    void dropFruitAboveBlock(int rowBase, int col);
};

#endif