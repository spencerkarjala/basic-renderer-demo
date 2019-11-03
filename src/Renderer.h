#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"
#include "Cube.h"
#include "Grid.h"
#include "Coordinate.h"

/*  Contains all necessary functions to draw the game board, any fruit placed
    on the game board, and any tetrominoes currently in play.                   */

class Renderer {

    public:

    Renderer(unsigned int shader);
    ~Renderer();
    void draw(Board* gameBoard, Grid* grid);
    void draw(Fruit* fruit);

    unsigned int getShader();
    void setShader(unsigned int shader);

    private:

    void drawFruit(Board* gameBoard);
    void drawTetrominoTrace(Board* gameBoard);
    void drawCell(Coordinate position);

    unsigned int shader;
    Cube* cube;
};

#endif