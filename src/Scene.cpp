#include <GL/glew.h>
#include <../lib/glm/gtc/type_ptr.hpp>
#include "Scene.h"

#include "stdio.h"

Scene::Scene() {
    this->camera = nullptr;
}

Scene::~Scene() {
    if (this->camera != nullptr) {
        delete this->camera;
    }
    glDeleteVertexArrays(1, &this->sceneData.VAO);
}

Scene::SceneData Scene::getSceneData() {
    return this->sceneData;
}

unsigned int Scene::getVAO() {
    return this->sceneData.VAO;
}

void Scene::setVAO(unsigned int VAO) {
    this->sceneData.VAO = VAO;
}

unsigned int Scene::getShader() {
    return this->sceneData.shader;
}

void Scene::setShader(unsigned int shader) {
    this->sceneData.shader = shader;
}

void Scene::insertObject(std::string key, WorldObject* object) {
    objects[key] = object;
}

void Scene::setupCamera(float* vectors, float fov, float aspect, float near, float far) {

    if (this->camera != nullptr) {
        delete this->camera;
    }

    glm::vec3  position  = glm::vec3(vectors[0], vectors[1], vectors[2]);
    glm::vec3  target    = glm::vec3(vectors[3], vectors[4], vectors[5]);
    glm::vec3  up        = glm::vec3(vectors[6], vectors[7], vectors[8]);

    this->camera = new Camera(position, target, up, fov, aspect, near, far);
}

void Scene::rotateCamera(float theta) {
    camera->rotate(theta, glm::vec3(0.f, 1.f, 0.f));
}

void Scene::setupBuffers() {

    unsigned int shader = this->sceneData.shader;
    this->sceneData.projection = glGetUniformLocation(shader, "Projection");
    this->sceneData.modelView  = glGetUniformLocation(shader, "ModelView");

    glGenVertexArrays(1, &this->sceneData.VAO);
    glBindVertexArray(this->sceneData.VAO);
}

void Scene::reshape(int width, int height) {

    if (camera == nullptr) {
        return;
    }
 
    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    // Update the aspect ratio for the frustum
    this->camera->setFOV((float) width / (float) height);

    // Set up perspective and modelView matrix for the frustum
    glm::mat4  mProjection  = camera->getProjection();
    glm::mat4  mModelView   = camera->getView() * glm::mat4(1.f);
    
    // Set model, view, and projection matrices
    glUniformMatrix4fv(this->sceneData.projection, 1, GL_TRUE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(this->sceneData.modelView,  1, GL_TRUE, glm::value_ptr(mModelView));
}

void Scene::refresh() {

    if (this->camera == nullptr) {
        return;
    }

    // Clear color and z-buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 mModelView   = camera->getView();
    glm::mat4 mProjection  = camera->getProjection();
    GLuint ModelView       = this->sceneData.modelView;
    GLuint Projection      = this->sceneData.projection;

    // Set up the shader and the MVP matrices
    glUseProgram(this->sceneData.shader);
    glUniformMatrix4fv(Projection, 1, false, glm::value_ptr(mProjection));
    glUniformMatrix4fv(ModelView,  1, false, glm::value_ptr(mModelView));
    glBindVertexArray(this->sceneData.VAO);

    std::unordered_map<std::string, WorldObject*>::iterator it = this->objects.begin();

    while (it != this->objects.end()) {
        WorldObject* curr = it->second;
        glm::mat4 model = mModelView * curr->getModel();
        glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(model));
        curr->draw(mModelView);
        it++;
    }
}