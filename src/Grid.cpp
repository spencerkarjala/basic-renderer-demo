#include "Grid.h"

Grid::Grid(int nRows, int nCols, Dimension size, Coordinate position) : WorldObject(position){
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