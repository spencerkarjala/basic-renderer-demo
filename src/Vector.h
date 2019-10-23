#ifndef VECTOR_H
#define VECTOR_H

/*  A simple data structure used to encode distance and direction information
    to simplify game logic in some places.                                      */

class Vector {
    public:
    Vector() {
        this->direction = 0;
        this->magnitude = 0;
    }
    Vector(int direction, int magnitude) {
        this->direction = direction;
        this->magnitude = magnitude;
    }
    ~Vector() {}

    int getDirection() {
        return this->direction;
    }
    void setDirection(int direction) {
        this->direction = direction % 360;
    }

    int getMagnitude() {
        return this->magnitude;
    }
    void setMagnitude(int magnitude) {
        this->magnitude = magnitude;
    }

    int cos(int angle) {

        int result = 0;
        switch (angle) {
            case 0:
                result = 1;
                break;
            case 45:
                result = 1;
                break;
            case 90:
                result = 0;
                break;
            case 135:
                result = -1;
                break;
            case 180:
                result = -1;
                break;
            case 225:
                result = -1;
                break;
            case 270:
                result = 0;
                break;
            case 315:
                result = 1;
                break;
        }
        return result;
    }

    int sin(int angle) {

        int result = 0;
        switch (angle) {
            case 0:
                result = 0;
                break;
            case 45:
                result = 1;
                break;
            case 90:
                result = 1;
                break;
            case 135:
                result = 1;
                break;
            case 180:
                result = 0;
                break;
            case 225:
                result = -1;
                break;
            case 270:
                result = -1;
                break;
            case 315:
                result = -1;
                break;
        }
        return result;
    }

    private:
    int direction;
    int magnitude;
};

#endif