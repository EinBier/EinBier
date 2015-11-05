#pragma once

#if defined(HAVE_PETSC)
#include <petsc.h>

typedef PetscScalar Scalar;
typedef PetscInt Int;

#define icomplex PETSC_i;
#define Real PetscRealPart
#define Imag PetscImaginaryPart

#endif

