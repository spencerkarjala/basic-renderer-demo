#include "BoardMediator.h"
#include <vector>

BoardMediator::BoardMediator() {}
BoardMediator::~BoardMediator() {}

// Starts a new tetromino from the top of the board
void BoardMediator::startTetrominoFromTop(Board* board) {

    if (board != nullptr) {

        // Determine the center position of the top of the board
        Coordinate pivotPosition;
        pivotPosition.x = board->getNumCols() / 2;
        pivotPosition.y = board->getNumRows() - 1;

        // Create a new tetromino with the cell size of the board
        Dimension cellSize = board->getCellSize();
        Tetromino* newTet  = this->generateRandomTetrominoType(pivotPosition, cellSize);

        // Throws an error if a collision is detected - used to end the game
        this->checkNewTetrominoPlacement(newTet, board);

        // Set up the new tetromino on the board since the position is valid
        board->setTetromino(newTet);
        this->placeTetrominoOnBoard(newTet, board);

        // Determine the random orientation the tetromino should be rotated to
        int numRotations = rand() % 4;

        // If the move is valid, drop the tetromino down one before rotation
        Vector displacement = Vector(270, 1);
        if (this->moveIsValid(displacement, newTet, board)) {
            this->moveTetromino(displacement, newTet, board);
        }

        // Rotate the tetromino a random number of times on 0..3
        for (int rot = 0; rot < numRotations; rot++) {
            this->rotateTetromino(newTet, board);
        }

        // Move the tetromino up twice if there is room to start it from the top
        this->moveTetromino(Vector(90, 1), newTet, board);
        this->moveTetromino(Vector(90, 1), newTet, board);
    }
}

// Used to move the tetromino in the direction indicated by the vector
void BoardMediator::moveTetromino(Vector displacement, Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        // If the move is valid, then move the tetromino in the desired direction
        if (this->moveIsValid(displacement, tetromino, board)) {
            tetromino->move(displacement);
        }

        // Otherwise, if the direction is downward, it must have hit something below so drop it
        else if (displacement.getDirection() == 270) {
            this->placeTetrominoOnBoard(tetromino, board);
            board->setTetromino(nullptr);
        }
    }
}

// Rotates the tetromino counterclockwise
void BoardMediator::rotateTetromino(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        // Remove the current tetromino from the board to prevent false collisions
        clearTetrominoFromBoard(tetromino, board);
        
        // If there's room to rotate, do it
        if (this->rotateIsValid(tetromino, board)) {
            tetromino->rotateCCW();
        }

        // Place the newly rotated tetromino back on the board
        this->placeTetrominoOnBoard(tetromino, board);
    }
}

// Drops the tetromino to place it in a permanent position on the board
void BoardMediator::dropTetromino(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {
        // Calculate the max distance that the tetromino can fall
        int dropDistance = this->findMaxFallingDistance(tetromino, board);

        // Drop the vector downward by whatever distance is found
        Vector dropVector(270, dropDistance);
        this->moveTetromino(dropVector, tetromino, board);

        // Clear lines and any combos if any were completed
        this->clearLines(tetromino, board);
        this->checkTetrominoForFruitCombo(tetromino, board);

        // Remove the tetromino from the board and delete its memory
        board->setTetromino(NULL);
        delete tetromino;

        // Start a new tetromino from the top now that it's been removed
        this->startTetrominoFromTop(board);
    }
}

// Places a tetromino on the board
void BoardMediator::placeTetrominoOnBoard(Tetromino* tetromino, Board* board) {

    // If the tetrominoes provided aren't available, then don't do anything
    if (tetromino == NULL || board->getTetromino() == NULL) {
        return;
    }

    const int NUM_FRUIT_PER_TETROMINO = 4;
    Fruit** tetrominoFruits = tetromino->getFruits();

    // Set each fruit in the tetromino on the board
    for (int index = 0; index < NUM_FRUIT_PER_TETROMINO; index++) {
        board->registerFruit(tetrominoFruits[index]);
    }
}

// Clears a tetromino from the board
void BoardMediator::clearTetrominoFromBoard(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        const int NUM_FRUIT_PER_TETROMINO = 4;
        Fruit** tetrominoFruits = tetromino->getFruits();

        // Remove each fruit in the tetromino from the board
        for (int index = 0; index < NUM_FRUIT_PER_TETROMINO; index++) {
            board->removeFruit(tetrominoFruits[index]);
        }
    }
}

// Clears any lines if they have been detected
void BoardMediator::clearLines(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        // Use the board to find any full rows present
        std::vector<int> fullRows = board->findFullRows();

        // Iterate over each full row and remove the fruit from them
        for (unsigned index = 0; index < fullRows.size(); index++) {

            // Subtract the index from the row to adjust for already-deleted rows
            int currRow = fullRows[index] - index;

            // In the case a fruit in the tetromino was cleared, make sure to remove it 
            for (unsigned fruit = 0; fruit < tetromino->NUM_FRUIT_PER_TETROMINO; fruit++) {
                if (tetromino->getFruits()[fruit] != NULL) {
                    if (tetromino->getFruits()[fruit]->getPosition().y == currRow) {
                        tetromino->getFruits()[fruit] = NULL;
                    }
                }
            }

            // Tell the board to clear out the current row
            board->clearRow(currRow);
        }
    }
}

// After moving the tetromino, check each fruit for a fruit combo
void BoardMediator::checkTetrominoForFruitCombo(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        Fruit** tetFruits = tetromino->getFruits();

        for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
            if (tetFruits[index] != NULL) {
                board->clearFruitCombos();
            }
        }
    }
}

// Generates a random type of tetromino to be placed on the board
Tetromino* BoardMediator::generateRandomTetrominoType(Coordinate pivotPosition, 
        Dimension size) {

    int randomTetrominoIndex = rand() % 4;
    Tetromino* newTetromino;

    // Depending on the random number, create one of four types
    switch (randomTetrominoIndex) {

        // Create an I-shaped tetromino, which has no special cases
        case 0:
            newTetromino = new TetrominoI(pivotPosition, size);
            break;

        // Create an L-shaped tetromino, and maybe flip it by its horizontal
        case 1:
            newTetromino = new TetrominoL(pivotPosition, size);
            if ((rand() % 2) == 0) {
                newTetromino->flipByHorizontal();
            }
            break;

        // Create an S-shaped tetromino, and maybe flip it by its horizontal
        case 2:
            newTetromino = new TetrominoS(pivotPosition, size);
            if ((rand() % 2) == 0) {
                newTetromino->flipByHorizontal();
            }
            break;

        // Create a T-shaped tetromino, which has no special cases
        case 3:
            newTetromino = new TetrominoT(pivotPosition, size);
            break;
    }
    return newTetromino;
}

// Find the max possible distance that a tetromino can fall
int BoardMediator::findMaxFallingDistance(Tetromino* tetromino, Board* board) {

    int maxFallDistance = 0;

    if (tetromino != nullptr && board != nullptr) {

        // Remove the tetromino from the board to prevent collision problems
        this->clearTetrominoFromBoard(tetromino, board);

        maxFallDistance = board->getNumRows();
        Fruit** tetFruits  = tetromino->getFruits();
        const int NUM_FRUITS_PER_TETROMINO = 4;

        // For each fruit, check below to see how far they can fall
        for (int index = 0; index < NUM_FRUITS_PER_TETROMINO; index++) {
            Fruit* currFruit = tetFruits[index];
            int currDistance = this->findMaxFallingDistance(currFruit, board);

            // If the current drop distance is lesser, save it
            if (currDistance < maxFallDistance) {
                maxFallDistance = currDistance;
            }
        }

        // Place the tetromino back on the board
        this->placeTetrominoOnBoard(tetromino, board);
    }

    return maxFallDistance;
}

// Given a fruit, find the max distance that it can fall before a collision
int BoardMediator::findMaxFallingDistance(Fruit* fruit, Board* board) {

    int maxFallDistance = 0;

    if (fruit != nullptr && board != nullptr) {

        Coordinate fruitPos  = fruit->getPosition();
        Fruit*** boardFruits = board->getFruits();
        maxFallDistance  = fruitPos.y;

        // Iterate over the cells below until a collision is detected
        for (int distance = 0; distance <= fruitPos.y; distance++) {
            if (boardFruits[fruitPos.y - distance][fruitPos.x] != NULL) {
                if (maxFallDistance >= distance) {
                    maxFallDistance = distance - 1;
                }
                break;
            }
        }
    }

    return maxFallDistance;
}

// Returns true if moving in the given direction has no collision or bounds issues
bool BoardMediator::moveIsValid(Vector displacement, Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        Fruit** tetFruits = tetromino->getFruits();
        int theta    = displacement.getDirection();
        int distance = displacement.getMagnitude();

        // Check each fruit in the tetromino to make sure that there is no collision
        for (int i = 0; i < tetromino->NUM_FRUIT_PER_TETROMINO; i++) {
            Coordinate fruitPos = tetFruits[i]->getPosition();
            fruitPos.x += distance * displacement.cos(theta);
            fruitPos.y += distance * displacement.sin(theta);

            // If a placement issue is detected, return false
            if (board->isOutOfBoundsAt(fruitPos) || board->isCollisionAt(fruitPos)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// Returns true if rotation has no collision or bounds issues
bool BoardMediator::rotateIsValid(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        Fruit** tetFruits   = tetromino->getFruits();
        Coordinate pivotPos = tetromino->getPosition();
        Coordinate newPos, fruitPos;

        // Check each fruit in the tetromino to make sure that there is no collision
        for (int i = 0; i < tetromino->NUM_FRUIT_PER_TETROMINO; i++) {
            fruitPos = tetFruits[i]->getPosition();
            newPos.x  = pivotPos.x - (fruitPos.y - pivotPos.y);
            newPos.y  = pivotPos.y + (fruitPos.x - pivotPos.x);

            // If a placement issue is detected, return false
            if (board->isOutOfBoundsAt(newPos) || board->isCollisionAt(newPos)) {
                return false;
            }
        }

        return true;
    }
    return false;
}

// Throws an error if placing the new tetromino has an issue - implements game ending
void BoardMediator::checkNewTetrominoPlacement(Tetromino* tetromino, Board* board) {

    if (tetromino != nullptr && board != nullptr) {

        for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
            Fruit* currFruit = tetromino->getFruits()[index];
            if (board->isCollisionAt(currFruit->getPosition())) {
                throw std::domain_error("Block collision - end game.");
            }
        }
    }
}

bool BoardMediator::isTetrominoCollisionAt(Tetromino* tetromino, Board* board, glm::vec3 position) {

    if (tetromino != nullptr && board != nullptr) {

        for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
            Fruit* currFruit = tetromino->getFruits()[index];
            if (board->isCollisionAt(currFruit->getPosition())) {
                return true;
            }
        }
        return false;
    }
    return true;        
}

void BoardMediator::updateTetrominoPosition(Board* board, glm::vec3 tetPos) {

    if (board != nullptr) {
    
        Tetromino* tetromino;
        Coordinate newPos;
        newPos.x = tetPos.x+4; newPos.y = tetPos.y; newPos.z = tetPos.z;
        
        if (!board->tetrominoIsOnBoard()) {
            Dimension cellSize = board->getCellSize();
            tetromino = this->generateRandomTetrominoType(newPos, cellSize);
            board->setTetromino(tetromino);
        }
        else {
            tetromino = board->getTetromino();
            tetromino->setPosition(newPos);
        }
    }
}