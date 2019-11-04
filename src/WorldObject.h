#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <../lib/glm/mat4x4.hpp>

class WorldObject {

    public:

    virtual ~WorldObject() {}

    virtual void draw(glm::mat4 mModelView) = 0;
    virtual glm::mat4 getModel() = 0;
};

#endif