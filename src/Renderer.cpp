#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Space3D.h"
#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

// Draws the game board with all lines, fruits, and tetrominoes
void Renderer::draw(Board* gameBoard, Grid* grid) {

    // this->drawTetrominoTrace(gameBoard);
    // this->drawFruit(gameBoard);
    // this->drawGrid(grid);
    this->drawRobotArm(grid);
}

// Draws a given fruit on the game board
void Renderer::draw(Fruit* fruit) {

    Color      color    = fruit->getColor();
    Coordinate position = fruit->getPosition();

    if (fruit != nullptr) {
        glColor4f(color.red, color.green, color.blue, color.alpha);
        this->drawCell(position);
    }
}

// Rotates the world coordinate system left
void Renderer::rotateWorldLeft(Board* gameBoard, Grid* grid) {

    glMatrixMode(GL_MODELVIEW);
    glRotatef(2.0, 0.0, 1.0, 0.0);
}

// Rotates the world coordinate system right
void Renderer::rotateWorldRight(Board* gameBoard, Grid* grid) {

    glMatrixMode(GL_MODELVIEW);
    glRotatef(-2.0, 0.0, 1.0, 0.0);
}

// Draws the grid for the game board
void Renderer::drawGrid(Grid* grid) {

    this->drawAxes();
    
    // glColor4f(1.0, 1.0, 1.0, 0.3);
    // this->drawGridHeight(grid);
    // this->drawGridWidth(grid);
    // this->drawGridDepth(grid);
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
                this->draw(currentFruit);
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

            //   the current fruit's color and set it to be transparent
            Fruit currFruit = Fruit(*tetFruits[index]);
            Color currColor = currFruit.getColor();
            currColor.alpha = 0.3;
            currFruit.setColor(currColor);

            // Get the current fruit's position and drop it by the max possible distance
            Coordinate currPos = currFruit.getPosition();
            currPos.y = currPos.y - maxFallDistance;
            currFruit.setPosition(currPos);

            // Draw the resulting fruit on the game board
            this->draw(&currFruit);
        }
    }
}

// Draws the coordinate axes for debugging
void Renderer::drawAxes() {

    // Save the current attributes (color)
    glPushAttrib(GL_CURRENT_BIT);

    glBegin(GL_LINES);
        // Draw the x-axis
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glVertex3f(-1000, 0, 0);
        glVertex3f(1000, 0, 0);

        // Draw the y-axis
        glColor4f(0.0, 1.0, 0.0, 1.0);
        glVertex3f(0, -1000, 0);
        glVertex3f(0, 1000, 0);

        // Draw the z-axis
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glVertex3f(0, 0, -1000);
        glVertex3f(0, 0, 1000);
    glEnd();

    // Restore attributes
    glPopAttrib();
}

// Renders lines in the xy-plane in 3D
void Renderer::drawGridHeight(Grid* grid) {
    
    // Obtain grid dimensions
    Dimension gridSize = grid->getSize();
    int numCols = grid->getNumCols();

    // Initialize increment variables based off grid size
    GLfloat xInc = numCols / gridSize.width;
    GLfloat zInc = 1       / gridSize.depth;

    // Initialize grid depth to center over z=0 axis
    double offset = gridSize.depth / 2;

    // Initialize grid depth to center over y=0 and z=0 axis
    double xOff = gridSize.width / 2;

    // Draw vertical lines along each discrete xy-plane combination
    glBegin(GL_LINES);
        for (GLfloat x = -xOff; x <= xOff; x += xInc) {
            for (GLfloat z = -offset; z <= offset; z += zInc) {
                glVertex3f(x, 0,               z);
                glVertex3f(x, gridSize.height, z);
            }
        }
    glEnd();
}

// Renders short grid lines depth-wise
void Renderer::drawGridDepth(Grid* grid) {
    
    // Obtain grid dimensions
    Dimension gridSize = grid->getSize();
    int numCols = grid->getNumCols();
    int numRows = grid->getNumRows();

    // Initialize increment variables based off grid size
    GLfloat xInc = numCols / gridSize.width;
    GLfloat yInc = numRows / gridSize.height;

    // Initialize grid depth to center over y=0 and z=0 axis
    double xOff = gridSize.width / 2;
    double zOff = gridSize.depth / 2;

    // Draw horizontal lines along each discrete xz-plane combination
    glBegin(GL_LINES);
        for (GLfloat x = -xOff; x <= xOff; x += xInc) {
            for (GLfloat y = 0; y <= gridSize.height; y += yInc) {
                glVertex3f(x, y, -zOff);
                glVertex3f(x, y, zOff);
            }
        }
    glEnd();
}

// Renders lines in the yz-plane in 3D
void Renderer::drawGridWidth(Grid* grid) {
    
    // Obtain grid dimensions
    Dimension gridSize = grid->getSize();
    int numRows = grid->getNumRows();

    // Initialize increment variables based off grid size
    GLfloat yInc = 1       / gridSize.depth;
    GLfloat zInc = numRows / gridSize.height;

    // Initialize grid depth to center over z=0 axis
    double xOff = gridSize.width / 2;
    double zOff = gridSize.depth / 2;

    // Draw horizontal lines along each discrete yz-plane combination
    glBegin(GL_LINES);
        for (GLfloat y = 0; y <= gridSize.height; y += yInc) {
            for (GLfloat z = -zOff; z <= zOff; z += zInc) {
                glVertex3f(-xOff, y, z);
                glVertex3f(xOff,  y, z);
            }
        }
    glEnd();
}

// Renders a cube in a particular cell 
void Renderer::drawCell(Coordinate position) {

    glPushMatrix();
    glTranslatef(position.x-4.5, position.y+0.5, 0);
    glutSolidCube(1);
    glPopMatrix();
}

void Renderer::drawRobotArm(Grid* grid) {

    // glUseProgram(grid->getShader());
    // glBindVertexArray(grid->getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);
}

// Used for debugging - prints modelView matrix
void Renderer::debugPrintModelView() {

    GLfloat model[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, model);

    printf("%0.7f, %0.7f, %0.7f, %0.7f\n", model[0], model[1], model[2], model[3]);
    printf("%0.7f, %0.7f, %0.7f, %0.7f\n", model[4], model[5], model[6], model[7]);
    printf("%0.7f, %0.7f, %0.7f, %0.7f\n\n", model[8], model[9], model[10], model[11]);
}

// Used for debugging - prints projection matrix
void Renderer::debugPrintProjection() {

    GLfloat proj[16];
    glGetFloatv(GL_PROJECTION_MATRIX, proj);

    printf("%0.7f, %0.7f, %0.7f, %0.7f\n", proj[0], proj[1], proj[2], proj[3]);
    printf("%0.7f, %0.7f, %0.7f, %0.7f\n", proj[4], proj[5], proj[6], proj[7]);
    printf("%0.7f, %0.7f, %0.7f, %0.7f\n\n", proj[8], proj[9], proj[10], proj[11]);
}