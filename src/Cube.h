#ifndef CUBE_H
#define CUBE_H

#include "SimpleObject.h"

class Cube : public SimpleObject {

    public:

    Cube(int flags, int type) : SimpleObject(flags, type) {}
    ~Cube() {}
};
#endif