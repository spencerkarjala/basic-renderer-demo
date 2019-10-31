#include <../lib/glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
               float fov, float aspect, float zNear, float zFar) {
    this->position  = position;
    this->up        = up;
    this->direction = target;
    this->fov       = fov;
    this->aspect    = aspect;
    this->zNear     = zNear;
    this->zFar      = zFar;
}

Camera::~Camera() {}

void Camera::setFOV(float fov) {
    this->fov = fov;
}

void Camera::setAspectRatio(float aspect) {
    this->aspect = aspect;
}

void Camera::setClippingPlanes(float zNear, float zFar) {
    this->zNear = zNear;
    this->zFar  = zFar;
}

glm::mat4 Camera::getView() {

    return glm::lookAt(position, direction, up);
}

glm::mat4 Camera::getProjection(){

    return glm::perspective(
        this->fov,
        this->aspect,
        this->zNear,
        this->zFar
    );
}