#ifndef CUBE_H
#define CUBE_H

#include "WorldObject.h"

class Cube : public WorldObject {

    public:

    Cube(int flags) : WorldObject(flags) {}
    ~Cube() {}
};
#endif