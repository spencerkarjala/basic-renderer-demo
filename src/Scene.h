#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include "Camera.h"
#include "WorldObject.h"

class Scene {

    public:

    struct SceneData {
        unsigned int VAO;
        unsigned int shader;
        unsigned int modelView;
        unsigned int projection;
    };

    Scene();
    ~Scene();

    SceneData getSceneData();

    unsigned int getVAO();
    void setVAO(unsigned int VAO);

    unsigned int getShader();
    void setShader(unsigned int shader);

    void insertObject(std::string key, WorldObject* object);

    void setupCamera(float* vectors, float fov, float aspect, float near, float far);
    void rotateCamera(float theta);

    void setupBuffers();

    void reshape(int width, int height);
    void refresh();

    private:

    SceneData sceneData;
    Camera* camera;
    std::unordered_map<std::string, WorldObject*> objects;
};

#endif