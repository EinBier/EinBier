#ifndef MATRIX_H
#define MATRIX_H

#include <Common/Type.h>

class Matrix
{
public:
    Coord shape;
    double **coeffs;

    Matrix(Coord);

    void insert(int, int, double);
    void Print();
};

#endif
