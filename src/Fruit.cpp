#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Fruit.h"

Fruit::Fruit() {
    this->position.x  = 0;
    this->position.y  = 0;
    this->size.width  = 0;
    this->size.height = 0;
    this->color       = this->generateRandomFruitColor();
}

Fruit::Fruit(int xPos, int yPos, int xSize, int ySize) {
    this->position.x  = xPos;
    this->position.y  = yPos;
    this->size.width  = xSize;
    this->size.height = ySize;
    this->color       = this->generateRandomFruitColor();
}

Fruit::Fruit(Coordinate newPosition, Dimension newSize) {
    this->position = newPosition;
    this->size     = newSize;
    this->color    = this->generateRandomFruitColor();
}

Fruit::Fruit(const Fruit& newFruit) {
    this->position = newFruit.position;
    this->size     = newFruit.size;
    this->color    = newFruit.color;
}

Fruit::~Fruit() {}

Coordinate Fruit::getPosition() {
    return this->position;
}

void Fruit::setPosition(Coordinate newPosition) {
    this->position = newPosition;
}

void Fruit::setPosition(int newPositionX, int newPositionY) {
    this->position.x = newPositionX;
    this->position.y = newPositionY;
}

Dimension Fruit::getSize() {
    return this->size;
}
void Fruit::setSize(Dimension newSize) {
    this->size = newSize;
}

Color Fruit::getColor() {
    return this->color;
}

void Fruit::setColor(Color newColor) {
    this->color = newColor;
}

void Fruit::setColor(float* newColor) {
    this->color.red   = newColor[0];
    this->color.green = newColor[1];
    this->color.blue  = newColor[2];
    this->color.alpha = newColor[3];
}

// Moves the fruit in the given direction
void Fruit::move(Vector displacement) {

    int theta    = displacement.getDirection();
    int distance = displacement.getMagnitude();

    // Add the distance in the required direction to the fruit position
    this->position.x += distance * displacement.cos(theta);
    this->position.y += distance * displacement.sin(theta);
}

// Generates a random color for fruit generation
Color Fruit::generateRandomFruitColor() {

    int randomFruitIndex = rand() % 5;
    Color newColor;
    switch (randomFruitIndex) {
        case 0: // Purple (grape)
            newColor.red   = 0.74510;
            newColor.green = 0.27843;
            newColor.blue  = 1.0;
            break;
        case 1: // Red (apple)
            newColor.red   = 0.91765;
            newColor.green = 0.14510;
            newColor.blue  = 0.27451;
            break;
        case 2: // Yellow (lemon)
            newColor.red   = 0.96863;
            newColor.green = 0.89020;
            newColor.blue  = 0.19216;
            break;
        case 3: // Green (pear)
            newColor.red   = 0.19608;
            newColor.green = 0.81961;
            newColor.blue  = 0.18824;
            break;
        case 4: // Orange (orange)
            newColor.red   = 0.95686;
            newColor.green = 0.54902;
            newColor.blue  = 0.18824;
            break;
    }
    newColor.alpha = 1.0;
    return newColor;
}