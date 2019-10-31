#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

std::string readFile(const char* path);
GLint succeeded(GLuint shader, GLenum flag);
void getAndPrintError(GLuint shader, std::string error);

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

    // Compile the vertex shader source; if it fails, error and quit
    glCompileShader(vertShader);
    if (!succeeded(vertShader, GL_COMPILE_STATUS)) {
        std::string errorMsg = vertPath;
        errorMsg += ", compilation failed:";
        getAndPrintError(vertShader, errorMsg);
        exit(0);
    }

    // Compile the fragment shader source; if it fails, error and quit
    glCompileShader(fragShader);
    if (!succeeded(fragShader, GL_COMPILE_STATUS)) {
        std::string errorMsg = vertPath;
        errorMsg += ", compilation failed:";
        getAndPrintError(fragShader, errorMsg);
        exit(0);
    }

    // Create the main shader program
    this->ID = glCreateProgram();

    // Attach the vertex and fragment shaders to the program
    glAttachShader(this->ID, vertShader);
    glAttachShader(this->ID, fragShader);

    // Link and initialize the main shader program; error if it fails
    glLinkProgram(this->ID);
    if (!succeeded(this->ID, GL_LINK_STATUS)) {
        std::string errorMsg = vertPath;
        errorMsg += ", linking failed:";
        getAndPrintError(this->ID, errorMsg);
        exit(0);
    }

    // // Set up the shader for use
    // glUseProgram(this->ID);

    // // Delete the now unneeded vertex and fragment shaders
    // glDeleteShader(vertShader);
    // glDeleteShader(fragShader);
}

Shader::~Shader() {}

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

// Returns 0 if an error is detected; other values otherwise
GLint succeeded(GLuint shader, GLenum flag) {

    GLint status;
    glGetShaderiv(shader, flag, &status);
    return status;
}

// In case of a shader build error, retrieves and prints error log
void getAndPrintError(GLuint shader, std::string error) {
    
    // Get the size of the error log
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    // Create an appropriately-sized char log
    char* logMsg = new char[logSize];

    // Get the error log and print it to cerr
    glGetShaderInfoLog(shader, logSize, NULL, logMsg);
    std::cerr << error << logMsg << std::endl;

    // Free neccessary memory
    delete[] logMsg;
}