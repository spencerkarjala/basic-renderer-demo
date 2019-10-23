#include "TetrominoI.h"
#include <math.h>
#include <stdio.h>

TetrominoI::TetrominoI(Coordinate position, Dimension size) : Tetromino(position, size) {

    this->orientation = 270;
    Coordinate fruitPositions[4] = {position, position, position, position};
    fruitPositions[0].x = position.x - 2;
    fruitPositions[1].x = position.x - 1;
    fruitPositions[3].x = position.x + 1;

    for (int index = 0; index < 4; index++) {
        this->fruits[index] = new Fruit(fruitPositions[index], size);
    }
}