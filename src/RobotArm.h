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
    void rotateTop(float theta);

    glm::vec3 getTipLocation(glm::mat4 mModelView);

    glm::mat4 getModel();

    void toggleHold();
    bool isHolding();

    private:
    
    Cube*  base;
    Cube*  bArm;
    Cube*  tArm;  
    Cube*  tip;
    bool   holding;
};

#endif