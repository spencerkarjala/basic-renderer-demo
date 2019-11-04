#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <../lib/glm/vec3.hpp>
#include <../lib/glm/mat4x4.hpp>

class Camera {

    public:

    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, 
            float fov, float aspect, float zNear, float zFar);
    ~Camera();

    void setFOV(float fov);
    void setAspectRatio(float aspect);
    void setClippingPlanes(float zNear, float zFar);
    void rotate(float angle, glm::vec3 axis);

    glm::mat4 getView();
    glm::mat4 getProjection();

    void printView();
    void printProjection();

    private:

    glm::mat4 view, projection;
    glm::vec3 position, up, direction;
    float fov, aspect, zNear, zFar;
};

#endif