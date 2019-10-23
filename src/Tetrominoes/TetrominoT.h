#ifndef TETROMINO_T_H
#define TETROMINO_T_H

#include "Tetromino.h"

class TetrominoT : public Tetromino {
    public:
    TetrominoT(Coordinate position, Dimension size);
    ~TetrominoT();
};

#endif