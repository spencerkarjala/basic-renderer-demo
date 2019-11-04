#include <GL/glew.h>
#include <../lib/glm/gtc/type_ptr.hpp>
#include "CompoundObject.h"
#include "stdio.h"

CompoundObject::CompoundObject() {}
CompoundObject::~CompoundObject() {}

void CompoundObject::draw(glm::mat4 mModelView) {
    glm::mat4 model = mModelView;
    printf("Whyyy\n");
        printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][0], model[1][0], model[2][0], model[3][0]);
        printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][1], model[1][1], model[2][1], model[3][1]);
        printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][2], model[1][2], model[2][2], model[3][2]);
        printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][3], model[1][3], model[2][3], model[3][3]);
        printf("\n");



    GLint shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

    // glm::mat4  model      = mModelView;
    GLuint     ModelView  = glGetUniformLocation(shader, "ModelView");
    SimpleObject* curr;

    glm::mat4 armModel = mModelView * this->objects[0]->getModel();
    glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(armModel));
    
    this->objects[0]->draw(mModelView);

    // for (unsigned index = 0; index < this->objects.size(); index++) {
    //     curr  = this->objects[index];
    //     model = model * curr->getModel();
    //     printf("i: %d\n", index);
    //     // printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][0], model[1][0], model[2][0], model[3][0]);
    //     // printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][1], model[1][1], model[2][1], model[3][1]);
    //     // printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][2], model[1][2], model[2][2], model[3][2]);
    //     // printf("%0.2f, %0.2f, %0.2f, %0.2f\n", model[0][3], model[1][3], model[2][3], model[3][3]);
    //     printf("\n");
    //     glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(model));
    //     curr->draw(model);
    // }
}

glm::mat4 CompoundObject::getModel() {

    if (this->objects.size() == 0) {
        printf("this really shouldn't happen\n");
        return glm::mat4(1.f);
    }
    return this->objects[0]->getModel();
}