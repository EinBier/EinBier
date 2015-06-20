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
    }
    printf("call Matrix constructor. [allocate+assign]\n");
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

Matrix Matrix::operator+(Matrix other)
{
	if (shape == other.shape) {
		Matrix add(shape);
		int i, j;
		for (i=0; i<add.shape.get_row(); i++) {
			for (j=0; j<add.shape.get_row(); j++) {
				add.coeffs[i][j] = coeffs[i][j] + other.coeffs[i][j];
			}
		}
		return add;
	} else {
		printf("Error: different shape\n");
		Matrix tmp(Coord(0, 0));
		return tmp;
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
	Matrix tmp(shape);
	int i, j;
	for (i=0; i<shape.get_row(); i++) {
		for (j=0; j<shape.get_row(); j++) {
			tmp.coeffs[i][j] = v * coeffs[i][j];
		}
	}
	return tmp;
}
