#include <../lib/glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
               float fov, float aspect, float zNear, float zFar) {
    this->position   = position;
    this->up         = up;
    this->direction  = target;
    this->fov        = fov;
    this->aspect     = aspect;
    this->zNear      = zNear;
    this->zFar       = zFar;
    this->view       = glm::lookAt(position, target, up);
    this->projection = glm::perspective(fov, aspect, zNear, zFar);
}

Camera::~Camera() {}

void Camera::setFOV(float fov) {
    this->fov = fov;
    this->projection = glm::perspective(fov, this->aspect, this->zNear, this->zFar);
}

void Camera::setAspectRatio(float aspect) {
    this->aspect = aspect;
    this->projection = glm::perspective(this->fov, aspect, this->zNear, this->zFar);
}

void Camera::setClippingPlanes(float zNear, float zFar) {
    this->zNear = zNear;
    this->zFar  = zFar;
    this->projection = glm::perspective(this->fov, this->aspect, zNear, zFar);
}

void Camera::rotate(float angle, glm::vec3 axis) {
    this->view = this->view * glm::rotate(glm::mat4(1.f), angle, axis);
}

glm::mat4 Camera::getView() {
    return this->view;
}

glm::mat4 Camera::getProjection(){
    return this->projection;
}