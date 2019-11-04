#ifndef NUMBER_H
#define NUMBER_H

#include "SimpleObject.h"

class Number : public SimpleObject {

    public:

    Number(int flags, int type) : SimpleObject(flags, type) {}
    ~Number() {}
};

#endif