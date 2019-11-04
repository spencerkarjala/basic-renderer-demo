#include "Grid.h"

Grid::Grid(int nRows, int nCols, Dimension size, int flags, int type) 
    : SimpleObject(flags, type){
    this->nRows = nRows;
    this->nCols = nCols;
    this->size  = size;
}

Grid::~Grid() {}

int Grid::getNumRows() {
    return this->nRows;
}

void Grid::setNumRows(int nRows) {
    this->nRows = nRows;
}

int Grid::getNumCols() {
    return this->nCols;
}

void Grid::setNumCols(int nCols) {
    this->nCols = nCols;
}

Dimension Grid::getSize() {
    return this->size;
}

void Grid::setSize(Dimension size) {
    this->size = size;
}

std::vector<float> Grid::getColors() {

    std::vector<float> colors;
    for (int index = 0; index < 120; index++) {
        colors.push_back(0.0f); colors.push_back(0.0f);
        colors.push_back(0.0f); colors.push_back(1.0f);
    }
    return colors;
}