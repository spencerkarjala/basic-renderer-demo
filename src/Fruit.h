#ifndef FRUIT_H
#define FRUIT_H

#include <stdint.h>
#include "Coordinate.h"
#include "Dimension.h"
#include "Color.h"
#include "Direction.h"
#include "Vector.h"

/*  Implements a simple fruit which occupies a given cell of the main game
    board. Contains state information and the ability to move given a vector.   */

class Fruit {

    public:
    Fruit();
    Fruit(int xPos, int yPos, int xSize, int ySize);
    Fruit(Coordinate newPosition, Dimension newSize);
    Fruit(const Fruit& newFruit);
    ~Fruit();

    Coordinate getPosition();
    void setPosition(Coordinate newPosition);
    void setPosition(int newPositionX, int newPositionY);

    Dimension getSize();
    void setSize(Dimension newSize);

    Color getColor();
    void setColor(Color newColor);
    void setColor(float* newColor);

    void move(Vector displacement);

    Color generateRandomFruitColor();
    
    private:
    Coordinate position;
    Dimension size;
    Color color;
};

#endif