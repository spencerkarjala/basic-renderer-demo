#ifndef SPACE3D_H
#define SPACE3D_H

#include "WorldObject.h"

class Space3D {

    public:

    struct Boundary {
        int x;
        int y;
        int z;
    };

    Space3D(Boundary boundary, Space3D* parent);
    ~Space3D();
    
    Boundary getBoundary();
    void setBoundary(Boundary boundary);

    Space3D* getParent();
    void     setParent(Space3D* parent);

    bool belongsTo(Space3D* parent);

    private:

    Boundary boundary;
    Space3D* parent;
    WorldObject* children;
};

#endif