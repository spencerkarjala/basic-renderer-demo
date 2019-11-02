#include <vector>
#include <GL/glew.h>
#include <../lib/glm/gtc/type_ptr.hpp>
#include "RobotArm.h"

#define B_ARM_HEIGHT 9.f
#define T_ARM_HEIGHT 9.f

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

RobotArm::RobotArm() {

    int flags  = WorldObject::OBJ_VERTEX | WorldObject::OBJ_FACE;
    this->base = new Cube(flags);
    this->bArm = new Cube(flags);
    this->tArm = new Cube(flags);
}
RobotArm::~RobotArm() {

    delete this->base;
    delete this->bArm;
    delete this->tArm;
}

void RobotArm::init(unsigned int shader) {

    this->base->setupBuffers("./src/cube.obj", "vPosition", "vColor", shader, baseColors);
    this->base->translate(glm::vec3(-8.f, 0.f, 0.f));
    this->base->scale(glm::vec3(4.f, 2.f, 4.f));

    this->bArm->setupBuffers("./src/cube.obj", "vPosition", "vColor", shader, bArmColors);
    this->bArm->translate(glm::vec3(0.f, (B_ARM_HEIGHT + 2.f) / 4, 0.f));
    this->bArm->scale(glm::vec3(0.25f, B_ARM_HEIGHT / 2.f, 0.25f));
    
    this->tArm->setupBuffers("./src/cube.obj", "vPosition", "vColor", shader, tArmColors);
    this->tArm->scale(glm::vec3(1.f, 1.f / B_ARM_HEIGHT, 1.f));
    this->tArm->rotate(-M_PI/2, glm::vec3(0.f, 0.f, 1.f));
    this->tArm->scale(glm::vec3(1.f, T_ARM_HEIGHT, 1.f));
    this->tArm->translate(glm::vec3(-B_ARM_HEIGHT / 2.f, 0.5f, 0.f));
    this->tArm->rotate(-M_PI/2, glm::vec3(0.f, 0.f, 1.f));

    // this->tArm->translate(glm::vec3(T_ARM_HEIGHT / 2.f, 0.5f, 0.f));
    // this->tArm->scale(glm::vec3(T_ARM_HEIGHT, 1.f / B_ARM_HEIGHT, 1.f));
}

glm::mat4 RobotArm::getModel() {
    return this->base->getModel();
}

void RobotArm::draw(unsigned int ModelView, glm::mat4 mModelView) {

    glm::mat4 armModel = mModelView * this->base->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(armModel));
    this->base->draw(GL_TRIANGLES);

    glm::mat4 bArmModel = armModel * this->bArm->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(bArmModel));
    this->bArm->draw(GL_TRIANGLES);

    glm::mat4 tArmModel = bArmModel * this->tArm->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(tArmModel));
    this->tArm->draw(GL_TRIANGLES);
}

void RobotArm::rotateBottom(float theta) {

    this->bArm->translate(glm::vec3(0.f, -B_ARM_HEIGHT / 2.f, 0.f));
    this->bArm->scale(glm::vec3(0.f, 2.F / B_ARM_HEIGHT, 0.f));
    this->bArm->rotate(theta, glm::vec3(0.f, 0.f, 1.f));
    // this->bArm->scale(glm::)
    // this->bArm->translate(glm::vec3());
}