#include "TetrominoL.h"
#include <math.h>
#include <stdio.h>

TetrominoL::TetrominoL(Coordinate position, Dimension size) : Tetromino(position, size) {

    this->orientation = 90;
    Coordinate fruitPositions[4] = {position, position, position, position};
    fruitPositions[0].x = position.x - 1;
    fruitPositions[0].y = position.y - 1;
    fruitPositions[1].x = position.x - 1;
    fruitPositions[3].x = position.x + 1;

    for (int index = 0; index < 4; index++) {
        this->fruits[index] = new Fruit(fruitPositions[index], size);
    }
}