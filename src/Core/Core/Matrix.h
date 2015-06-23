#pragma once

#include <Common/Type.h>

class Matrix
{
public:
    Shape shape;
    double **coeffs;

    Matrix(Shape);

    void insert(int, int, double);
    void Print();
    Matrix  operator+(Matrix);
    Matrix  operator-();
    Matrix  operator-(Matrix);
    Matrix  operator*(double);
};
