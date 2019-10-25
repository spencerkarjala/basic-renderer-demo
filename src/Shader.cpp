#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "Shader.h"

std::string readFile(const char* path);

//todo: add error handling for missing shaders
Shader::Shader(const char* vertPath, const char* fragPath) {

    // Read the shader's vertex and fragment filedata from disk
    std::string vertData = readFile(vertPath);
    std::string fragData = readFile(fragPath);

    // Convert vertex and fragment filedata to GL character format
    const GLchar* vertDataC = vertData.c_str();
    const GLchar* fragDataC = fragData.c_str();

    // Create the vertex and fragment shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Initialize the vertex and fragment shader source code
    glShaderSource(vertShader, 1, &vertDataC, NULL);
    glShaderSource(fragShader, 1, &fragDataC, NULL);

    // Compile the vertex and fragment shaders' source code
    glCompileShader(vertShader);
    glCompileShader(fragShader);

    // Create the main shader program
    this->ID = glCreateProgram();

    // Attach the vertex and fragment shaders to the program
    glAttachShader(this->ID, vertShader);
    glAttachShader(this->ID, fragShader);

    // Link and initialize the main shader program
    glLinkProgram(this->ID);
    glUseProgram(this->ID);

    // Delete the now unneeded vertex and fragment shaders
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}



unsigned int Shader::getID() {
    return this->ID;
}

void Shader::setID(unsigned int ID) {
    this->ID = ID;
}

// Returns a string containing shader file data from disk
std::string readFile(const char* path) {

    std::stringstream dataStream;
    std::ifstream file(path);
    dataStream << file.rdbuf();

    return dataStream.str();
}