#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <vector>
#include <../lib/glm/mat4x4.hpp>
#include <../lib/glm/vec3.hpp>
#include "Coordinate.h"

class WorldObject {

    public:

    enum {
        OBJ_VERTEX = 1,
        OBJ_NORM   = 2,
        OBJ_LINE   = 4,
        OBJ_FACE   = 8
    };

    WorldObject(int flags);
    ~WorldObject();

    glm::mat4 getModel();

    void scale(glm::vec3 scaleVec);
    void translate(glm::vec3 transVec);

    void setupBuffers(const char* path, const char* vAttrS, const char* cAttrS,
        unsigned int shader, std::vector<float> colors);
        
    void draw(unsigned int type);

    private:

    int flags;
    unsigned int vboVertex, vboColor, ebo, attrVertex, attrColor;
    glm::mat4 model;
};

#endif