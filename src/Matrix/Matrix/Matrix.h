#pragma once

#include <Common/Type.h>

#if defined(HAVE_PETSC)
#include <petsc.h>
#endif


class Matrix
{
private:
    int m_row;
    int m_col;
    Mat m_mat;

public:
    Matrix();
    ~Matrix();
    Mat* getMat(){return &m_mat;}
    void setSize(int row, int col);
    void setValue(int i, int j, Scalar);
    void assemble();
    void Print();

    Scalar getValue(int i, int j);
};
