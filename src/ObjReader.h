#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <fstream>
#include <../lib/glm/vec4.hpp>
#include <vector>

class ObjReader {

    public:

    ObjReader();
    ~ObjReader();

    std::vector<glm::vec4> readVertices(const char* path);

    private:

    std::fstream openObjFile(const char* path);
};

#endif