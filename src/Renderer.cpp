#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#include "Renderer.h"

Renderer::Renderer(unsigned int shader) {
    // this->shader = shader;
    // this->cube = new Cube(SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_FACE);
    // cube->setupBuffers("./src/cube.obj", "vPosition", "vColor", shader, cubeColors);
}
Renderer::~Renderer() {}

// Draws the game board with all lines, fruits, and tetrominoes
void Renderer::draw(Board* gameBoard, Grid* grid) {

    // this->drawTetrominoTrace(gameBoard);
    // this->drawFruit(gameBoard);
    // this->drawRobotArm(grid);
}

// // Draws a given fruit on the game board
// void Renderer::draw(Fruit* fruit) {

//     Color      color    = fruit->getColor();
//     Coordinate position = fruit->getPosition();

//     if (fruit != nullptr) {
//         glColor4f(color.red, color.green, color.blue, color.alpha);
//         this->drawCell(position);
//     }
// }

// // Draws each fruit on the game board that isn't NULL
// void Renderer::drawFruit(Board* gameBoard) {

//     // Get fruit matrix and dimensions for traversal
//     Fruit*** fruits = gameBoard->getFruits();
//     int numRows = gameBoard->getNumRows();
//     int numCols = gameBoard->getNumCols();

//     // For each cell in each row, if a fruit exists there, draw it
//     for (int row = 0; row < numRows; row++) {
//         for (int col = 0; col < numCols; col++) {
//             Fruit* currentFruit = fruits[row][col];
//             if (currentFruit != NULL) {
//                 this->draw(currentFruit);
//             }
//         }
//     }
// }

// // Draws a simple trace below the board's tetromino to guide the user
// void Renderer::drawTetrominoTrace(Board* gameBoard) {

//     Tetromino* tetromino = gameBoard->getTetromino();

//     if (tetromino != NULL) {

//         // Obtain the max distance below the current tetromino to drop
//         int maxFallDistance = gameBoard->findMaxFallDistance(tetromino);
//         Fruit** tetFruits = tetromino->getFruits();

//         // For each fruit in the tetromino, draw them as far below as possible
//         for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {

//             //   the current fruit's color and set it to be transparent
//             Fruit currFruit = Fruit(*tetFruits[index]);
//             Color currColor = currFruit.getColor();
//             currColor.alpha = 0.3;
//             currFruit.setColor(currColor);

//             // Get the current fruit's position and drop it by the max possible distance
//             Coordinate currPos = currFruit.getPosition();
//             currPos.y = currPos.y - maxFallDistance;
//             currFruit.setPosition(currPos);

//             // Draw the resulting fruit on the game board
//             this->draw(&currFruit);
//         }
//     }
// }

// // Renders a cube in a particular cell 
// void Renderer::drawCell(Coordinate position) {

//     glPushMatrix();
//     glTranslatef(position.x-4.5, position.y+0.5, 0);
//     glutSolidCube(1);
//     glPopMatrix();
// }