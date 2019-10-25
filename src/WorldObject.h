#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "Coordinate.h"

class WorldObject {

    public:

    WorldObject(Coordinate position);
    ~WorldObject();

    Coordinate getPosition();
    void setPosition(Coordinate position);
    void setPosition(int x, int y, int z);

    private:

    Coordinate position;
};

#endif