#include <GL/glew.h>
#include <../lib/glm/gtc/matrix_transform.hpp>
#include "WorldObject.h"
#include "ObjReader.h"

WorldObject::WorldObject(int flags) {
    this->model = glm::mat4(1.f);
    this->flags = flags;
}

WorldObject::~WorldObject() {}

glm::mat4 WorldObject::getModel() {
    return this->model;
}

void WorldObject::scale(glm::vec3 scaleVec) {
    this->model = glm::scale(this->model, scaleVec);
}

void WorldObject::translate(glm::vec3 transVec) {
    this->model = glm::translate(this->model, transVec);
}

void WorldObject::rotate(float theta, glm::vec3 axis) {
    this->model = glm::rotate(this->model, theta, axis);
}

void WorldObject::setupBuffers(const char* path, const char* vAttrS, 
    const char* cAttrS, unsigned int shader, std::vector<float> colors) {

    // Read .obj data from file at given path
    ObjReader reader;
    ObjReader::ObjData data = reader.readValues(path);

    // Retrieve necessary values from the struct
    std::vector<float>        vertices = data.vertices;
    std::vector<unsigned int> indices;
    if ((this->flags & OBJ_LINE) != 0) {
        indices = data.lineIndices;
    }
    else if ((this->flags & OBJ_FACE) != 0) {
        indices = data.faceIndices;
    }

    // Bind, fill, and locate the buffer used for vertex data
    glGenBuffers(1, &this->vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboVertex);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(float) * vertices.size(),
        &vertices[0],
        GL_STATIC_DRAW);
    this->attrVertex = glGetAttribLocation(shader, vAttrS);

    // Bind, fill, and locate the buffer used for color data
    glGenBuffers(1, &this->vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboColor);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * colors.size(),
        &colors[0],
        GL_STATIC_DRAW);
    this->attrColor = glGetAttribLocation(shader, cAttrS);

    // Bind and fill the buffers used for line index data
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * indices.size(),
        &indices[0],
        GL_STATIC_DRAW);
}

void WorldObject::draw(unsigned int type) {

    GLint bufSize = 0;
    
    // Load vertex attributes and bind the vertex buffer
    glEnableVertexAttribArray(this->attrVertex); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vboVertex);
    glVertexAttribPointer(this->attrVertex, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Load color attributes and bind the color buffer
    glEnableVertexAttribArray(this->attrColor);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboColor);
    glVertexAttribPointer(this->attrColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind the buffer used to index the vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    // Get the current buffer size <=> number of vertices to draw
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufSize);

    // Draw the object
    glDrawElements(type, bufSize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}