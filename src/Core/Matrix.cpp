#include <stdio.h>
#include <stdlib.h>

#include <Common/Type.h>
#include <Core/Matrix.h>

Matrix::Matrix(Coord s)
{
    shape = s;
    coeffs = (double**)calloc(shape.get_row(), sizeof(double*));
    int i;
    for (i=0; i<shape.get_row(); i++) {
        coeffs[i] = (double*)calloc(shape.get_col(), sizeof(double));
    }
    int mem = shape.get_row()*shape.get_col()*sizeof(double);
    printf("Matrix allocator: %dx%d = %d bytes [%p]\n",
           shape.get_row(), shape.get_col(), mem, this);

}

void Matrix::insert(int r, int c, double v)
{
    coeffs[r][c] = v;
    return;
}

void Matrix::Print()
{
    int r = shape.get_row();
    int c = shape.get_col();
    int i, j;
    printf("Matrix: %d , %d \t [%p]\n", r, c, this);
    for (i=0; i<r; i++) {
        printf("[ ");
        for (j=0; j<r; j++) {
            printf(" %g ", coeffs[i][j]);
        }
        printf(" ]\n");
    }
}

Matrix Matrix::operator+(Matrix other)
{
    if (shape == other.shape) {
        Matrix *add = new Matrix(shape);
        int i, j;
        for (i=0; i<add->shape.get_row(); i++) {
            for (j=0; j<add->shape.get_col(); j++) {
                add->coeffs[i][j] = coeffs[i][j] + other.coeffs[i][j];
            }
        }
        return *add;
    } else {
        printf("Error: different shape\n");
        Matrix *tmp = new Matrix(Coord(0, 0));
        return *tmp;
    }
}

Matrix Matrix::operator-()
{
    Matrix tmp(shape);
    int i, j;
    for (i=0; i<shape.get_row(); i++) {
            for (j=0; j<shape.get_row(); j++) {
                tmp.coeffs[i][j] = -coeffs[i][j];
            }
        }
        return tmp;
}

Matrix Matrix::operator-(Matrix other)
{
    return *this + (-other);
}

Matrix Matrix::operator*(double v)
{
    Matrix *tmp = new Matrix(shape);
    int i, j;
    for (i=0; i<shape.get_row(); i++) {
        for (j=0; j<shape.get_row(); j++) {
            tmp->coeffs[i][j] = v * coeffs[i][j];
        }
    }
    return *tmp;
}
