#include <iostream>
#include "ObjReader.h"

ObjReader::ObjReader() {}

ObjReader::~ObjReader() {}

std::vector<glm::vec4> ObjReader::readVertices(const char* path) {

    std::vector<glm::vec4> vertices;

    try {

        std::fstream file = this->openObjFile(path);
        char* line = new char[256];
        glm::vec4 vertex;
        
        while (!file.eof()) {
            file.getline(line, 256);
            sscanf(line, "v %f %f %f %f", &vertex.x, &vertex.y, &vertex.z, &vertex.w);
            if (line[0] == 'v' && line[1] == ' ') {
                vertices.push_back(vertex);
            }
        }

        file.close();
    }
    catch (std::runtime_error err) {

        std::cerr << err.what() << ": " << path << std::endl;
    }
    
    return vertices;
}

std::fstream ObjReader::openObjFile(const char* path) {

    std::fstream newFile;
    newFile.open(path, std::fstream::in);

    if (!newFile.good()) {
        throw std::runtime_error("Failed to open file");
    }

    return newFile;
}