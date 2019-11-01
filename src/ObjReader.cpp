#include <iostream>
#include <../lib/glm/vec4.hpp>
#include "ObjReader.h"

int getObjLineType(const char* line);

ObjReader::ObjReader() {}

ObjReader::~ObjReader() {}

// Reads any necessary data from a .obj file
ObjReader::ObjData ObjReader::readValues(const char* path) {

    ObjData values;
    std::fstream file;
    char* line = new char[256];

    // Try to open the obj file at the given path
    try {
        file = this->openObjFile(path);
    }

    // If path was invalid, throw error and quit
    catch (std::runtime_error err) {
        std::cerr << err.what() << ": " << path << std::endl;
        exit(0);
    }
    
    while (!file.eof()) {

        // Get next line in file and figure out what type it is
        file.getline(line, 256);
        int type = getObjLineType(line);

        // If can't figure out what type it is, throw error
        if (type == -1) {
            std::cerr << "Error in .obj: " << line << std::endl;
        }

        switch (type) {

            case OBJ_VERTEX:
                this->readVertex(line, &values);    continue;
            case OBJ_NORM:
                /* todo */                          continue;
            case OBJ_LINE:
                this->readLine(line, &values);      continue;
            case OBJ_FACE:
                this->readFace(line, &values);      continue;
            case OBJ_IGNORE:
                continue;
        }
    }

    delete[] line;
    file.close();
    
    return values;
}

// Opens an obj file given a path
std::fstream ObjReader::openObjFile(const char* path) {

    // Initialize the filestream and open it
    std::fstream newFile;
    newFile.open(path, std::fstream::in);

    // If an error occurred (eg. bad path), throw an error
    if (!newFile.good()) {
        throw std::runtime_error("Failed to open file");
    }

    return newFile;
}

// Retrieves vertex data at a given line in a .obj file
void ObjReader::readVertex(char* line, ObjReader::ObjData* values) {

    // Retrieve the 4 data points from file
    glm::vec4 vertex;
    sscanf(line, "v %f %f %f %f", &vertex.x, &vertex.y, &vertex.z, &vertex.w);

    // Save the retrieved data
    values->vertices.push_back(vertex.x);
    values->vertices.push_back(vertex.y);
    values->vertices.push_back(vertex.z);
    values->vertices.push_back(vertex.w);
}

void ObjReader::readNorm(char* line, ObjReader::ObjData* values) {
    //todo
}
void ObjReader::readLine(char* line, ObjReader::ObjData* values) {
    
    unsigned int index1, index2;
    sscanf(line, "l %d %d", &index1, &index2);

    values->lineIndices.push_back(index1-1);
    values->lineIndices.push_back(index2-1);
}
void ObjReader::readFace(char* line, ObjData* values) {
    
    unsigned int index1, index2, index3;
    sscanf(line, "f %d %d %d", &index1, &index2, &index3);

    values->faceIndices.push_back(index1-1);
    values->faceIndices.push_back(index2-1);
    values->faceIndices.push_back(index3-1);
}

// Determines what the current line's data type is
int getObjLineType(const char* line) {

    // If line begins with "v ", mark it a vertex
    if (line[0] == 'v' && line[1] == ' ') {
        return ObjReader::OBJ_VERTEX;
    }

    // If line begins with "vn ", mark it a vertex normal
    else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
        return ObjReader::OBJ_NORM;
    }

    // If line begins with "l ", mark it a line
    else if (line[0] == 'l' && line[1] == ' ') {
        return ObjReader::OBJ_LINE;
    }

    // If line begins with "f ", mark it a face
    else if (line[0] == 'f' && line[1] == ' ') {
        return ObjReader::OBJ_FACE;
    }

    // If line is empty or is a comment, ignore it
    else if (line[0] == '#' || line[0] == 13) {
        return ObjReader::OBJ_IGNORE;
    }

    return -1;
}