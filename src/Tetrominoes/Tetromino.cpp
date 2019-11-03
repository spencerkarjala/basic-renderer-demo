#include <stdio.h>
#include <math.h>
#include "Tetromino.h"

Tetromino::Tetromino(Coordinate position, Dimension size) {
    this->position = position;
}

Tetromino::Tetromino(Tetromino* newTetromino) {
    this->position = newTetromino->position;
    for (int index = 0; index < this->NUM_FRUIT_PER_TETROMINO; index++) {
        this->fruits[index] = new Fruit(*newTetromino->fruits[index]);
    }
}

Coordinate Tetromino::getPosition() {
    return this->position;
}

void Tetromino::setPosition(Coordinate newPosition) {

    Coordinate oldPosition = this->position;
    this->position = newPosition;
    Coordinate diff;
    diff.x = newPosition.x - oldPosition.x;
    diff.y = newPosition.y - oldPosition.y;

    for (int index = 0; index < this->NUM_FRUIT_PER_TETROMINO; index++) {
        Fruit* curr = this->fruits[index];
        Coordinate fruitPos = curr->getPosition();
        fruitPos.x = fruitPos.x + diff.x;
        fruitPos.y = fruitPos.y + diff.y;
        curr->setPosition(fruitPos);
    }
}

int Tetromino::getOrientation() {
    return this->orientation;
}
void Tetromino::setOrientation(int orientation) {
    this->orientation = orientation;
}

Fruit** Tetromino::getFruits() {
    return this->fruits;
}

// Flips the tetromino by its horizontal for orientation randomization
void Tetromino::flipByHorizontal() {

    // Flip each fruit in the tetromino by the pivot position
    for (int index = 0; index < 4; index++) {
        Fruit* fruitToFlip = this->fruits[index];
        Coordinate newPosition = fruitToFlip->getPosition();
        newPosition.x += 2 * (this->fruits[2]->getPosition().x - newPosition.x);
        fruitToFlip->setPosition(newPosition);
    }
}

// Moves the tetromino in the direction specified by a vector
void Tetromino::move(Vector displacement) {
    
    // Move each fruit in the direction specified
    Fruit backupFruits[4];
    for (int index = 0; index < 4; index++) {
        Fruit* currentFruit = this->fruits[index];
        backupFruits[index] = *currentFruit;
        currentFruit->move(displacement);
    }

    // Set the new position of the pivot point of the tetromino
    this->position = this->fruits[2]->getPosition();
}

// Rotates the tetromino counterclockwise by 90 degrees
void Tetromino::rotateCCW() {

    Coordinate pivotPos = this->fruits[2]->getPosition();
    Coordinate rotatePos, newPos;

    // Rotate each fruit by 90 degrees in the tetromino
    for (int index = 0; index < 4; index++) {

        Fruit* fruitToRotate = this->fruits[index];
        rotatePos = fruitToRotate->getPosition();

        // Calculate the new positions based off of the pivot position
        newPos.x  = pivotPos.x - (rotatePos.y - pivotPos.y);
        newPos.y  = pivotPos.y + (rotatePos.x - pivotPos.x);
        fruitToRotate->setPosition(newPos);
    }

    // Set the new orientation of the tetromino
    this->orientation = (this->orientation + 90) % 360;
}

// Returns true if the given fruit is contained in the tetromino; false otherwise
bool Tetromino::fruitBelongsToTetromino(Fruit* fruit) {

    // Run over all fruits in the tetromino and compare their addresses
    for (int index = 0; index < this->NUM_FRUIT_PER_TETROMINO; index++) {
        if (fruit == this->fruits[index]) {
            return true;
        }
    }
    return false;
}