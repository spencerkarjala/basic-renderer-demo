#ifndef BOARD_MEDIATOR_H
#define BOARD_MEDIATOR_H

#include <stdexcept>
#include "Board.h"
#include "Direction.h"
#include "Color.h"
#include "Vector.h"

/*  Implements behavior to mediate between tetromino pieces and the game board
    itself to abstract features of both away from one another. This includes 
    operations like bounds checking, fruit placement, collision checking, etc.  */

class BoardMediator {

    public:
    BoardMediator();
    ~BoardMediator();
    
    void startTetrominoFromTop(Board* board);
    void moveTetromino(Vector move, Tetromino* tetromino, Board* board);
    void rotateTetromino(Tetromino* tetromino, Board* board);
    void dropTetromino(Tetromino* tetromino, Board* board);
    int findMaxFallingDistance(Tetromino* tetromino, Board* board);

    private:
    void placeTetrominoOnBoard(Tetromino* tetromino, Board* board);
    void clearTetrominoFromBoard(Tetromino* tetromino, Board* board);
    void clearLines(Tetromino* tetromino, Board* board);
    void checkTetrominoForFruitCombo(Tetromino* tetromino, Board* board);
    Tetromino* generateRandomTetrominoType(Coordinate pivotPosition, Dimension size);
    int findMaxFallingDistance(Fruit* fruit, Board* board);
    Vector* determineMoveDirection(Tetromino* tetromino, Board* board);
    bool moveIsValid(Vector displacement, Tetromino* tetromino, Board* board);
    bool rotateIsValid(Tetromino* tetromino, Board* board);
    void checkNewTetrominoPlacement(Tetromino* tetromino, Board* board);
};

#endif