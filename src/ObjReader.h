#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <fstream>
#include <vector>

class ObjReader {

    public:

    enum {
        OBJ_VERTEX = 1,
        OBJ_NORM   = 2,
        OBJ_LINE   = 4,
        OBJ_FACE   = 8,
        OBJ_IGNORE = 16
    };

    struct ObjData {
        std::vector<float>    vertices;
        std::vector<unsigned> lineIndices;
        std::vector<unsigned> faceIndices;
    };

    ObjReader();
    ~ObjReader();

    ObjData readValues(const char* path);

    private:

    std::fstream openObjFile(const char* path);
    void readVertex(char* line, ObjData* values);
    void readNorm(char* line, ObjData* values);
    void readLine(char* line, ObjData* values);
    void readFace(char* line, ObjData* values);
};

#endif