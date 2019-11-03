#include <stdexcept>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Board.h"

// Simple local operation to add a coordinate and a vector
Coordinate operator+(const Coordinate& A, Vector B) {
    Coordinate result;
    result.x = A.x + B.getMagnitude() * Vector().cos(B.getDirection());
    result.y = A.y + B.getMagnitude() * Vector().sin(B.getDirection());
    return result;
}

// Simple local operation to subtract a coordinate and a vector
Coordinate operator-(const Coordinate& A, Vector B) {
    Coordinate result;
    result.x = A.x - B.getMagnitude() * Vector().cos(B.getDirection());
    result.y = A.y - B.getMagnitude() * Vector().sin(B.getDirection());
    return result;
}

// Simple local operation to obtain the scalar multiple of a vector
Vector operator*(const int& A, Vector& B) {
    Vector newVector = Vector(B.getDirection(),0);
    newVector.setMagnitude(A * B.getMagnitude());
    return newVector;
}

// Basic constructor
Board::Board(int width, int height, int numCols, int numRows, int xPosition, int yPosition) {
    this->boardSize.width  = width;
    this->boardSize.height = height;
    this->numCols          = numCols;
    this->numRows          = numRows;
    this->cellSize.width   = ((float) width) / ((float) numCols);
    this->cellSize.height  = ((float) height) / ((float) numRows);
    this->position.x       = xPosition;
    this->position.y       = yPosition;
    this->gridColor.red    = 0.5;
    this->gridColor.green  = 0.5;
    this->gridColor.blue   = 0.5;
    this->gridColor.alpha  = 1.0;
    this->tetromino        = NULL;

    // Initialize the game board to be a NULL array
    this->fruits = new Fruit**[numRows];
    for (int row = 0; row < numRows; row++) {
        this->fruits[row] = new Fruit*[numCols];
        for (int col = 0; col < numCols; col++) {
            this->fruits[row][col] = NULL;
        }
    }
}

// Delete any memory used by the game board's array
Board::~Board() {
    if (this->fruits != NULL) {
        for (int row = 0; row < this->numRows; row++) {
            for (int col = 0; col < this->numCols; col++) {
                if (this->fruits[row][col] != NULL) {
                    delete this->fruits[row][col];
                }
            }
            delete[] fruits[row];
        }
        delete[] fruits;
    }
}

Dimension Board::getBoardSize() {
    return this->boardSize;
}
void Board::setBoardSize(Dimension newBoardSize) {
    this->boardSize = newBoardSize;
}

Dimension Board::getCellSize() {
    return this->cellSize;
}
void Board::setCellSize(Dimension newCellSize) {
    this->cellSize = newCellSize;
}

int Board::getNumCols() {
    return this->numCols;
}
void Board::setNumCols(const int numCols) {
    this->numCols = numCols;
}

int Board::getNumRows() {
    return this->numRows;
}
void Board::setNumRows(const int numRows) {
    this->numRows = numRows;
}

Coordinate Board::getPosition() {
    return this->position;
}
void Board::setPosition(Coordinate newPosition) {
    this->position = newPosition;
}
void Board::setPosition(int newPositionX, int newPositionY) {
    this->position.x = newPositionX;
    this->position.y = newPositionY;
}

Color Board::getGridColor() {
    return this->gridColor;
}
void Board::setGridColor(Color newGridColor) {
    this->gridColor = newGridColor;
}

Tetromino* Board::getTetromino() {
    return this->tetromino;
}
void Board::setTetromino(Tetromino* tetromino) {
    
    this->tetromino = tetromino;
}

Fruit*** Board::getFruits() {
    return this->fruits;
}

// Sets up a new fruit on the game board
void Board::registerFruit(Fruit* newFruit) {

    Coordinate newFruitPosition = newFruit->getPosition();
    if (this->isOutOfBoundsAt(newFruitPosition)) {
        return;
    } else if (!this->isCollisionAt(newFruitPosition)) {
        this->fruits[newFruitPosition.y][newFruitPosition.x] = newFruit;
    }
}

// Removes a fruit from the game board
void Board::removeFruit(Fruit* fruitToRemove) {

    Coordinate fruitPosition = fruitToRemove->getPosition();
    int xPos = fruitPosition.x;
    int yPos = fruitPosition.y;
    if (xPos >= 0 && xPos < this->getNumCols() && yPos >= 0 && yPos < this->getNumRows()) {
        this->fruits[fruitPosition.y][fruitPosition.x] = NULL;
    }
}

// Returns true if a tetromino exists on the board; false otherwise
bool Board::tetrominoIsOnBoard() {
    if (this->tetromino == NULL) {
        return 0;
    }
    return 1;
}

// Returns true if the given coordinate is out of bounds; false otherwise
bool Board::isOutOfBoundsAt(Coordinate fruitPos) {
    int xPos = fruitPos.x;
    int yPos = fruitPos.y;
    if (xPos < 0 || yPos < 0 || xPos >= this->numCols || yPos >= this->numRows) {
        return true;
    }
    return false;
}

// Returns true if a fruit is detected at the given coordinate; false otherwise
bool Board::isCollisionAt(Coordinate fruitPos) {
    if (this->fruits[fruitPos.y][fruitPos.x] != NULL) {
        return true;
    }
    return false;
}

// Returns true if the cell contains no fruit at the given coordinate; false otherwise
bool Board::isEmptyAt(Coordinate fruitPos) {
    return !this->isCollisionAt(fruitPos);
}

// In case of a game over, runs over the whole cell array and sets all colors to grey
void Board::setCellsToGray() {

    // Create a new 'grey' color
    Color newColor;
    newColor.red   = 0.85;
    newColor.green = 0.85;
    newColor.blue  = 0.85;
    newColor.alpha = 0.5;

    // Iterate over the whole grid and set each fruit color to grey
    for (int row = 0; row < this->numRows; row++) {
        for (int col = 0; col < this->numCols; col++) {
            Fruit* currFruit = this->fruits[row][col];
            if (currFruit != NULL) {
                currFruit->setColor(newColor);
            }
        }
    }
}

// Returns a vector of locations of any rows that are full
std::vector<int> Board::findFullRows() {

    std::vector<int> fullRows = std::vector<int>();
    bool rowFull;

    // Runs over each row, checking each column to see if it has a fruit
    for (int row = 0; row < this->numRows; row++) {
        rowFull = true;

        // If any column is empty, break out and mark row as non-full
        for (int col = 0; col < this->numCols; col++) {
            if (this->fruits[row][col] == NULL) {
                rowFull = false;
                break;
            }
        }

        // If row was full, save this row number
        if (rowFull == true) {
            fullRows.push_back(row);
        }
    }

    return fullRows;
}

// Clears out a row once it has been found to be full
void Board::clearRow(int row) {

    for (int col = 0; col < this->numCols; col++) {
        delete this->fruits[row][col];
        this->fruits[row][col] = NULL;
        this->dropFruitAboveBlock(row, col);
    }
}

// Checks the entire grid array to see if a 1x3 fruit combo exists
void Board::clearFruitCombos() {

    uint8_t maxForward  = 0;
    uint8_t maxBackward = 0;
    bool    comboFound  = false;
    Coordinate currPos, frontPos, backPos;

    // Loops over each row and column as a center point to check
    for (uint8_t row = 0; row < this->numRows; row++) {
        for (uint8_t col = 0; col < this->numCols; col++) {

            // At each cell in the grid, check all 4 directions nearby
            for (uint8_t direction = 0; direction < 4; direction++) {
                
                // Set up the current direction vector
                Vector currDir(direction * 45, 1);

                currPos.x = col;
                currPos.y = row;
                frontPos  = currPos + currDir;
                backPos   = currPos - currDir;

                // If this cell isn't empty, check forward and backward in the current
                // direction and find the maximum number of same-colored fruit 
                if (!this->isEmptyAt(currPos)) {
                    maxForward  = this->findComboLength(frontPos, currDir, 0);
                    maxBackward = this->findComboLength(backPos, -1 * currDir, 0);
                }

                // If 3 fruit in a row are found, clear them from the grid
                if (maxForward + maxBackward >= 2) {

                    // Calculate the position of the first fruit in the combo
                    Coordinate removePosition = currPos - (maxBackward * currDir);
                    Fruit* toRemove[3];

                    // Run across the combo and get each fruit to remove
                    for (int index = 0; index < 3; index++) {
                        toRemove[index] = this->fruits[removePosition.y][removePosition.x];
                        removePosition = removePosition + currDir;
                    }

                    // Delete each fruit in the combo and drop all above down by one cell
                    for (int index = 0; index < 3; index++) {
                        Coordinate abc = toRemove[index]->getPosition();
                        this->dropFruitAboveBlock(abc.y, abc.x);
                        delete toRemove[index];
                    }

                    comboFound = true;
                }
                maxForward = maxBackward = 0;
            }
        }
    }

    // If a combo was found, then some fruit must have moved - check everything again
    if (comboFound == true) {
        this->clearFruitCombos();
    }
}

// Recursively determines the length of a combo at the given position and direction
int Board::findComboLength(Coordinate position, Vector direction, int depth) {

    // If three similar-colored fruit have been found in a row, return the depth
    if (depth == 2) {
        return depth;
    }
    // Otherwise, if the cell is out of bounds or empty, quit recursion
    else if (this->isOutOfBoundsAt(position) || this->isEmptyAt(position)) {
        return depth;
    }

    // Calculate the previous position, color, and current color
    Coordinate prevPosition = position - direction;
    Color prevColor = this->fruits[position.y][position.x]->getColor();
    Color currColor = this->fruits[prevPosition.y][prevPosition.x]->getColor();

    // If the two colors are equal, then recursively check the next one
    if (prevColor == currColor) {
        return this->findComboLength(position + direction, direction, depth + 1);
    }

    // Return the found length of the fruit combo
    return depth;
}

// Obtains the max distance that a tetromino can fall before collision
int Board::findMaxFallDistance(Tetromino* tetromino) {

    int maxFallDistance = this->getNumRows();
    Fruit** tetFruits  = tetromino->getFruits();

    // For each fruit in the tetromino, determine the max distance they can fall
    for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {

        // Find the max distance the current fruit can fall
        Fruit* currFruit = tetFruits[index];
        int currDistance = this->findMaxFallingDistance(currFruit, tetromino);

        // If the found distance is more restrictive, it's the max distance to fall
        if (currDistance < maxFallDistance) {
            maxFallDistance = currDistance;
        }
    }
    
    return maxFallDistance;
}

// Given a single fruit, finds the max distance that it can fall
int Board::findMaxFallingDistance(Fruit* fruit, Tetromino* tetromino) {

    Coordinate fruitPos  = fruit->getPosition();
    Fruit*** boardFruits = this->fruits;
    int distance         = 0;

    // Move down until a cell is found that doesn't belong to the tetromino
    while(tetromino->fruitBelongsToTetromino(fruit) && distance < fruitPos.y) {
        distance++;
        fruit = boardFruits[fruitPos.y - distance][fruitPos.x];
    }

    // Once free from the tetromino, count empty blocks below the fruit
    while (fruit == NULL && distance < fruitPos.y) {
        distance++;
        fruit = boardFruits[fruitPos.y - distance][fruitPos.x];
    }

    // Subtract one from the distance in case of bounds
    if (fruit != NULL && distance != 0) {
        distance--;
    }
    
    return distance;
}

// Given a coordinate, drop each fruit above it by one to remove it
void Board::dropFruitAboveBlock(int rowBase, int col) {

    // For each row above the coordinate, drop the fruit
    for (int row = rowBase; row < this->numRows-1; row++) {
        Fruit* currFruit    = this->fruits[row+1][col];
        if (currFruit != NULL) {
            Coordinate fruitPos = currFruit->getPosition();
            fruitPos.y          = fruitPos.y - 1;
            currFruit->setPosition(fruitPos);
        }
        this->fruits[row][col]   = currFruit;
        this->fruits[row+1][col] = NULL;
    }
}