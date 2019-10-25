#include "Space3D.h"

Space3D::Space3D(Boundary boundary, Space3D* parent) {
    this->boundary = boundary;
    this->parent   = parent;
}

Space3D::~Space3D() {}

Space3D::Boundary Space3D::getBoundary() {
    return this->boundary;
}

void Space3D::setBoundary(Boundary boundary) {
    this->boundary = boundary;
}

Space3D* Space3D::getParent() {
    return this->parent;
}

void Space3D::setParent(Space3D* parent) {
    this->parent = parent;
}

bool Space3D::belongsTo(Space3D* parent) {
    if (this->parent == parent) {
        return true;
    }
    return false;
}