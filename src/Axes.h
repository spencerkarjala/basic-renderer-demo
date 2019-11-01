#ifndef AXES_H
#define AXES_H

#include "WorldObject.h"

class Axes : public WorldObject {

    public:

    Axes(int flags) : WorldObject(flags) {}
    ~Axes() {}
};

#endif