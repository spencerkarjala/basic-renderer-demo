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

    unsigned int getVAO();
    void setVAO(unsigned int VAO);

    unsigned int getShader();
    void setShader(unsigned int shader);

    private:

    Coordinate position;
    unsigned int VAO;
    unsigned int shader;
};

#endif