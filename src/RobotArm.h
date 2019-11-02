#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <../lib/glm/mat4x4.hpp>
#include "Cube.h"

class RobotArm {

    public:

    RobotArm();
    ~RobotArm();

    void init(unsigned int shader);
    void draw(unsigned int ModelView, glm::mat4 mModelView);
    void rotateBottom(float theta);

    glm::mat4 getModel();

    private:
    
    Cube* base;
    Cube* bArm;
    Cube* tArm;
};

#endif