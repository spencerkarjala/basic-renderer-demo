#ifndef GRID_H
#define GRID_H

#include "WorldObject.h"
#include "Dimension.h"

class Grid : public WorldObject {
    
    public:

    Grid(int nRows, int nCols, Dimension size, int flags);
    ~Grid();

    int getNumRows();
    void setNumRows(int nRows);

    int getNumCols();
    void setNumCols(int nCols);

    Dimension getSize();
    void setSize(Dimension size);

    std::vector<float> getColors();

    private:

    int nRows, nCols;
    Dimension size;
};

#endif