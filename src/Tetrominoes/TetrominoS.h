#ifndef TETROMINO_S_H
#define TETROminO_S_H

#include "Tetromino.h"

class TetrominoS : public Tetromino {
    public:
    TetrominoS(Coordinate position, Dimension size);
    ~TetrominoS();
};

#endif