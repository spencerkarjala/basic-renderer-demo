#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <vector>
#include <../lib/glm/vec3.hpp>
#include <../lib/glm/vec4.hpp>
#include "Coordinate.h"
#include "WorldObject.h"

class SimpleObject : public WorldObject {

    public:

    enum {
        OBJ_VERTEX = 1,
        OBJ_NORM   = 2,
        OBJ_LINE   = 4,
        OBJ_FACE   = 8
    };

    SimpleObject();
    SimpleObject(int flags, int type);
    ~SimpleObject();

    void setFlags(int flags);

    glm::mat4 getModel();

    void scale(glm::vec3 scaleVec);
    void translate(glm::vec3 transVec);
    void rotate(float theta, glm::vec3 axis);

    void setupBuffers(const char* path, const char* vAttrS, const char* cAttrS,
        unsigned int shader, glm::vec4 color);
        
    void swapModel(const char* path);
    void swapColor(std::vector<float> colors);

    void draw(glm::mat4 mModelView);

    private:

    int flags;
    unsigned int vboVertex, vboColor, ebo, attrVertex, attrColor;
    int type;
    glm::mat4 model;
};

#endif