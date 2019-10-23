#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"

/*  Contains all necessary functions to draw the game board, any fruit placed
    on the game board, and any tetrominoes currently in play.                   */

class Renderer {
    public:
    Renderer();
    ~Renderer();
    void draw(Board* gameBoard);
    void draw(Fruit fruit, Board* gameBoard);

    private:
    void drawGrid(Board* gameBoard);
    void drawFruit(Board* gameBoard);
    void drawTetrominoTrace(Board* gameBoard);
};

#endif