#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <../lib/glm/mat4x4.hpp>
#include "Cube.h"
#include "CompoundObject.h"

class RobotArm {

    public:

    RobotArm(unsigned int flags, unsigned int type);
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

// #ifndef ROBOT_ARM_H
// #define ROBOT_ARM_H

// #include <../lib/glm/mat4x4.hpp>
// #include "Cube.h"
// #include "CompoundObject.h"

// class RobotArm : public CompoundObject {

//     public:

//     RobotArm(unsigned int flags, unsigned int type);
//     ~RobotArm();

//     void init(unsigned int shader);
//     // void draw(glm::mat4 mModelView);
//     void rotateBottom(float theta);
//     void rotateTop(float theta);

//     glm::vec3 getTipLocation(glm::mat4 mModelView);

//     glm::mat4 getModel();

//     void toggleHold();
//     bool isHolding();

//     private:
//     glm::mat4 model;
//     Cube*  base;
//     Cube*  bArm;
//     Cube*  tArm;  
//     Cube*  tip;
//     bool   holding;
// };

// #endif