#ifndef AXES_H
#define AXES_H

#include "SimpleObject.h"

class Axes : public SimpleObject {

    public:

    Axes(int flags, int type) : SimpleObject(flags, type) {}
    ~Axes() {}
};

#endif