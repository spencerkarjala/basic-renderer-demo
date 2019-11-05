#include <vector>
#include <GL/glew.h>
#include <../lib/glm/gtc/type_ptr.hpp>
#include <../lib/glm/vec4.hpp>
#include "RobotArm.h"

#include "stdio.h"

#define NUM_OBJECTS  4
#define B_ARM_HEIGHT 11.f
#define T_ARM_HEIGHT 12.f

std::vector<float> baseColors {
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
};

std::vector<float> tArmColors {
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f
};

std::vector<float> bArmColors {
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

// RobotArm::RobotArm(unsigned int flags, unsigned int type) : CompoundObject() {
RobotArm::RobotArm(unsigned int flags, unsigned int type) {

    flags      = SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_FACE;
    this->base     = new Cube(flags, GL_TRIANGLES);
    this->bArm     = new Cube(flags, GL_TRIANGLES);
    this->tArm     = new Cube(flags, GL_TRIANGLES);
    this->tip      = new Cube(flags, GL_TRIANGLES);
    this->holding  = false;
}

RobotArm::~RobotArm() {
    delete this->base;
    delete this->bArm;
    delete this->tArm;
    delete this->tip;
}

void RobotArm::init(unsigned int shader) {
    
    this->base->setupBuffers("./src/cube.obj", "vPosition", "vColor", 
        shader, glm::vec4(1.f, 0.f, 0.f, 1.f));
    this->base->translate(glm::vec3(-8.f, 0.f, 0.f));
    this->base->scale(glm::vec3(4.f, 2.f, 4.f));

    this->bArm->setupBuffers("./src/cube.obj", "vPosition", "vColor",
        shader, glm::vec4(0.f, 0.f, 1.f, 1.f));
    this->bArm->translate(glm::vec3(0.f, (B_ARM_HEIGHT + 2.f) / 4, 0.f));
    this->bArm->scale(glm::vec3(0.25f, B_ARM_HEIGHT / 2.f, 0.25f));
    
    this->tArm->setupBuffers("./src/cube.obj", "vPosition", "vColor",
        shader, glm::vec4(0.f, 1.f, 0.f, 1.f));
    this->tArm->translate(glm::vec3(T_ARM_HEIGHT / 2.f, 0.5f, 0.f));
    this->tArm->scale(glm::vec3(T_ARM_HEIGHT, 1.f / B_ARM_HEIGHT, 1.f));
    
    this->tip->setupBuffers("./src/cube.obj", "vPosition", "vColor",
        shader, glm::vec4(0.f, 0.f, 0.f, 1.f));
    this->tip->translate(glm::vec3(0.5f, 0.f, 0.f));
    this->tip->scale(glm::vec3(1.f / T_ARM_HEIGHT, 1.f, 1.f));
}

glm::vec3 RobotArm::getTipLocation(glm::mat4 mModelView) {

    glm::mat4  baseModel  = this->base->getModel();
    glm::mat4  bArmModel  = this->bArm->getModel();
    glm::mat4  tArmModel  = this->tArm->getModel();
    glm::mat4  tipModel   = this->tip->getModel();
    glm::vec4  loc = baseModel*bArmModel*tArmModel*tipModel*glm::vec4(0.f,0.f,0.f,1.f);

    return glm::vec3(floor(loc.x), floor(loc.y), floor(loc.z));
}

glm::mat4 RobotArm::getModel() {
    return this->base->getModel();
}

void RobotArm::draw(unsigned int ModelView, glm::mat4 mModelView) {

    glm::mat4 armModel = mModelView * this->base->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(armModel));
    this->base->draw(armModel);

    glm::mat4 bArmModel = armModel * this->bArm->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(bArmModel));
    this->bArm->draw(armModel);

    glm::mat4 tArmModel = bArmModel * this->tArm->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(tArmModel));
    this->tArm->draw(armModel);

    // glm::mat4 tipModel = tArmModel * this->tip->getModel();
    // glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(tipModel));
    // this->tip->draw();
}

void RobotArm::rotateBottom(float theta) {

    this->bArm->scale(glm::vec3(1.f, 1.f / B_ARM_HEIGHT, 1.f));
    this->bArm->translate(glm::vec3(0.f, -B_ARM_HEIGHT / 2.f, 0.f));
    this->bArm->rotate(theta, glm::vec3(0.f, 0.f, 1.f));
    this->bArm->translate(glm::vec3(0.f, B_ARM_HEIGHT / 2.f, 0.f));
    this->bArm->scale(glm::vec3(1.f, B_ARM_HEIGHT, 1.f));
}

void RobotArm::rotateTop(float theta) {

    this->tArm->scale(glm::vec3(1.f / T_ARM_HEIGHT, 1.f, 1.f));
    this->tArm->translate(glm::vec3(-T_ARM_HEIGHT / 2.f, 0.f, 0.f));
    this->tArm->rotate(theta, glm::vec3(0.f, 0.f, 1.f));
    this->tArm->translate(glm::vec3(T_ARM_HEIGHT / 2.f, 0.f, 0.f));
    this->tArm->scale(glm::vec3(T_ARM_HEIGHT, 1.f, 1.f));
}

void RobotArm::toggleHold() {
    this->holding = this->holding ^ 1;
}

bool RobotArm::isHolding() {
    return this->holding;
}