#include "WorldObject.h"

WorldObject::WorldObject(Coordinate position) {
    this->position = position;
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