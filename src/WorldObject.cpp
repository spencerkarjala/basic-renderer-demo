#include "WorldObject.h"

WorldObject::WorldObject(Coordinate position) {
    this->position = position;
    this->VAO      = 0;
    this->shader   = 0;
}

WorldObject::~WorldObject() {}

Coordinate WorldObject::getPosition() {
    return this->position;
}

void WorldObject::setPosition(Coordinate position) {
    this->position = position;
}

void WorldObject::setPosition(int x, int y, int z) {
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

unsigned int WorldObject::getVAO() {
    return this->VAO;
}

void WorldObject::setVAO(unsigned int VAO) {
    this->VAO = VAO;
}

unsigned int WorldObject::getShader() {
    return this->shader;
}

void WorldObject::setShader(unsigned int shader) {
    this->shader = shader;
}