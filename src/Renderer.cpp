#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

// Draws the game board with all lines, fruits, and tetrominoes
void Renderer::draw(Board* gameBoard) {
    this->drawTetrominoTrace(gameBoard);
    this->drawFruit(gameBoard);
    this->drawGrid(gameBoard);
}

// Draws a given fruit on the game board
void Renderer::draw(Fruit fruit, Board* gameBoard) {
    Color      fruitColor = fruit.getColor();
    Dimension  fruitSize  = fruit.getSize();
    Coordinate fruitPos   = fruit.getPosition();
    Coordinate boardPos   = gameBoard->getPosition();

    // Calculate the dimensions of the polygon to render
    int xMin = boardPos.x + round((float) fruitPos.x * fruitSize.width);
    int xMax = boardPos.x + round((float) (fruitPos.x + 1) * fruitSize.width);
    int yMin = boardPos.y + round((float) fruitPos.y * fruitSize.height);
    int yMax = boardPos.y + round((float) (fruitPos.y + 1) * fruitSize.height);
    
    // Render the polygon with the above dimensions
    glColor4f(fruitColor.red, fruitColor.green, fruitColor.blue, fruitColor.alpha);
    glBegin(GL_POLYGON);
        glVertex2i(xMin, yMin);
        glVertex2i(xMin, yMax);
        glVertex2i(xMax, yMax);
        glVertex2i(xMax, yMin);
    glEnd();
}

// Draws the grid for the game board
void Renderer::drawGrid(Board* gameBoard) {

    // Get grid color from board and set the color for rendering
    Color gridColor = gameBoard->getGridColor();
    glColor3f(gridColor.red, gridColor.green, gridColor.blue);

    // Get the board's total size, cell size, and position relative to window's bottom-left
    Dimension  boardSize = gameBoard->getBoardSize();
    Dimension  cellSize  = gameBoard->getCellSize();
    Coordinate boardPos  = gameBoard->getPosition();

    // Initialize the indices used to iterate through the column & row lines
    float xIndex = boardPos.x;
    float yIndex = boardPos.y;

    // Calculate the max pixel value to stop drawing lines at
    int xMax = boardSize.width + boardPos.x;
    int yMax = boardSize.height + boardPos.y;

    // Draw each vertical line for the board along the dimensions determined above
    while (round(xIndex) <= xMax) {
        glBegin(GL_LINES);
            glVertex2i(round(xIndex), boardPos.y);
            glVertex2i(round(xIndex), yMax);
        glEnd();
        xIndex += cellSize.width;
    }

    // Draw each horizontal line for the board along the dimensions determined above
    while (round(yIndex) <= yMax) {
        glBegin(GL_LINES);
            glVertex2i(boardPos.x, round(yIndex));
            glVertex2i(xMax, round(yIndex));
        glEnd();
        yIndex += cellSize.height;
    }
}

// Draws each fruit on the game board that isn't NULL
void Renderer::drawFruit(Board* gameBoard) {

    // Get fruit matrix and dimensions for traversal
    Fruit*** fruits = gameBoard->getFruits();
    int numRows = gameBoard->getNumRows();
    int numCols = gameBoard->getNumCols();

    // For each cell in each row, if a fruit exists there, draw it
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Fruit* currentFruit = fruits[row][col];
            if (currentFruit != NULL) {
                this->draw(*currentFruit, gameBoard);
            }
        }
    }
}

// Draws a simple trace below the board's tetromino to guide the user
void Renderer::drawTetrominoTrace(Board* gameBoard) {

    Tetromino* tetromino = gameBoard->getTetromino();

    if (tetromino != NULL) {

        // Obtain the max distance below the current tetromino to drop
        int maxFallDistance = gameBoard->findMaxFallDistance(tetromino);
        Fruit** tetFruits = tetromino->getFruits();

        // For each fruit in the tetromino, draw them as far below as possible
        for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {

            // Get the current fruit's color and set it to be transparent
            Fruit currFruit = Fruit(*tetFruits[index]);
            Color currColor = currFruit.getColor();
            currColor.alpha = 0.3;
            currFruit.setColor(currColor);

            // Get the current fruit's position and drop it by the max possible distance
            Coordinate currPos = currFruit.getPosition();
            currPos.y = currPos.y - maxFallDistance;
            currFruit.setPosition(currPos);

            // Draw the resulting fruit on the game board
            this->draw(currFruit, gameBoard);
        }
    }
}