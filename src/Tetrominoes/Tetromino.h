#ifndef TETROMINO_H
#define TETROMINO_H
#include <stdint.h>
#include "../Color.h"
#include "../Fruit.h"
#include "../Direction.h"
#include "../Vector.h"

/*  Implements the tetromino - a grouping of four blocks made popular by the 
    game Tetris. Implements simple behavior like moving and rotation.           */

class Tetromino {

    public:
    const uint8_t NUM_FRUIT_PER_TETROMINO = 4;

    Tetromino(Coordinate position, Dimension size);
    Tetromino(Tetromino* newTetromino);

    Coordinate getPosition();
    void setPosition(Coordinate newPosition);

    int getOrientation();
    void setOrientation(int orientation);

    Fruit** getFruits();

    void move(Vector displacement);
    
    void flipByHorizontal();

    void rotateCCW();
    bool fruitBelongsToTetromino(Fruit* fruit);

    protected:
    Coordinate position;
    Fruit* fruits[4];
    int orientation;
};

#endif