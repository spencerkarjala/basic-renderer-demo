#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
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
    std::cout << "VERTEX:\n" << vertDataC << std::endl;
    // const GLchar* fragDataC = fragData.c_str();
    // std::cout << "FRAGMENT:\n" << fragDataC << std::endl;

    // Create the vertex and fragment shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLint status;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &status );
    printf("mooooOOOORE GARBAGE %d\n", status);
    // GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Initialize the vertex and fragment shader source code
    glShaderSource(vertShader, 1, &vertDataC, NULL);
    // glShaderSource(fragShader, 1, &fragDataC, NULL);

    // Compile the vertex and fragment shaders' source code
    glCompileShader(vertShader);

    /*
     *
     * 
     * 
     * 
     * 
     * 
     *  delete this fucking garbage */

    GLint compiled;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    printf("COMPILED 1 %d\n", compiled);
    if (!compiled) {
        std::cerr << vertPath << ", compilation failed: " << std::endl;
        GLint logSize;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logSize);
        printf("logsize: %d\n", logSize);
        char* logMsg = new char[logSize];
        glGetShaderInfoLog(vertShader, logSize, NULL, logMsg);
        std::cerr << logMsg << std::endl;
        delete[] logMsg;

        exit(0);
    }


    // glCompileShader(fragShader);

    // /*
    //  *
    //  * 
    //  * 
    //  * 
    //  * 
    //  * 
    //  *  delete this fucking garbage */

    // glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    // printf("COMPILED 2 %d\n", compiled);
    // if (!compiled) {
    //     std::cerr << vertPath << ", compilation failed: " << std::endl;
    //     GLint logSize;
    //     glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logSize);
    //     char* logMsg = new char[logSize];
    //     glGetShaderInfoLog(fragShader, logSize, NULL, logMsg);
    //     std::cerr << logMsg << std::endl;
    //     delete[] logMsg;

    //     exit(0);
    // }

    // Create the main shader program
    this->ID = glCreateProgram();

    // Attach the vertex and fragment shaders to the program
    glAttachShader(this->ID, vertShader);
    // glAttachShader(this->ID, fragShader);

    // Link and initialize the main shader program
    glLinkProgram(this->ID);

    /*
     *
     * 
     * 
     * 
     * 
     * 
     *  delete this fucking garbage */

    GLint  linked;
    glGetProgramiv( this->ID, GL_LINK_STATUS, &linked );
    printf("LINKED %d\n", linked);
    if ( !linked ) {
        std::cerr << "Shader program failed to link" << std::endl;
        GLint  logSize;
        glGetProgramiv( this->ID, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog( this->ID, logSize, NULL, logMsg );
        std::cerr << logMsg << std::endl;
        delete [] logMsg;

        exit( 0 );
    }
    glUseProgram(this->ID);

    // Delete the now unneeded vertex and fragment shaders
        glDeleteShader(vertShader);
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