#ifndef COMPOUND_OBJECT_H
#define COMPOUND_OBJECT_H

#include <vector>
#include "SimpleObject.h"

class CompoundObject : public WorldObject {

    public:

    CompoundObject();
    ~CompoundObject();

    void draw(glm::mat4 mModelView);
    glm::mat4 getModel();

    protected:

    std::vector<SimpleObject*> objects;
};

#endif