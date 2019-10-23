#ifndef TETROMINO_I_H
#define TETROMINO_I_H

#include "Tetromino.h"

class TetrominoI : public Tetromino {
    public:
    TetrominoI(Coordinate position, Dimension size);
    ~TetrominoI();
};

#endif