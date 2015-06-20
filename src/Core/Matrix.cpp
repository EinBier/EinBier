#include <stdio.h>
#include <stdlib.h>

#include <Common/Type.h>
#include <Core/Matrix.h>

Matrix::Matrix(Coord s)
{
    shape = s;
    coeffs = (double**)malloc(shape.get_row() * sizeof(double*));
    int i;
    for (i=0; i<shape.get_row(); i++) {
        coeffs[i] = (double*)malloc(shape.get_col() * sizeof(double));
        printf("%d ", i);
    }
    printf("end\n");
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
    printf("Matrix: %d , %d\n", r, c);
    for (i=0; i<r; i++) {
        printf("[ ");
        for (j=0; j<r; j++) {
            printf(" %g ", coeffs[i][j]);
        }
        printf(" ]\n");
    }
}
