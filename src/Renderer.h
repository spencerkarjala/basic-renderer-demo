#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"
#include "Grid.h"
#include "Coordinate.h"

/*  Contains all necessary functions to draw the game board, any fruit placed
    on the game board, and any tetrominoes currently in play.                   */

class Renderer {
    public:
    Renderer();
    ~Renderer();
    void draw(Board* gameBoard, Grid* grid);
    void draw(Fruit* fruit);
    void rotateWorldLeft(Board* gameBoard, Grid* grid);
    void rotateWorldRight(Board* gameBoard, Grid* grid);

    private:
    void drawGrid(Grid* grid);
    void drawFruit(Board* gameBoard);
    void drawTetrominoTrace(Board* gameBoard);
    void drawAxes();
    void drawGridHeight(Grid* grid);
    void drawGridDepth(Grid* grid);
    void drawGridWidth(Grid* grid);
    void drawCell(Coordinate position);
    void drawRobotArm(Grid* grid);
    void debugPrintModelView();
    void debugPrintProjection();
};

#endif