#ifndef TETROMINO_L_H
#define TETROMINO_L_H

#include "Tetromino.h"

class TetrominoL : public Tetromino {
    public:
    TetrominoL(Coordinate position, Dimension size);
    ~TetrominoL();
};

#endif