#include <Matrix/Matrix.h>
#include <Common/Message.h>

#if defined(HAVE_PETSC)
#include <petsc.h>
#endif

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
//    MatDestroy(&m_mat);
}

void Matrix::setSize(int row, int col)
{
    m_row=row;
    m_col=col;
//    MatCreateSeqDense(PETSC_COMM_SELF, row, col, NULL, &m_mat);
    MatCreate(PETSC_COMM_SELF, &m_mat);
	//MatSetSizes(m_mat, row, col, PETSC_DETERMINE, PETSC_DETERMINE);
}


void Matrix::setValue(int i, int j, double r)
{
//    MatSetValue(m_mat, i, j, r, INSERT_VALUES);
}

void Matrix::assemble(){
//    MatAssemblyBegin(m_mat, MAT_FINAL_ASSEMBLY);
    //   MatAssemblyEnd(m_mat, MAT_FINAL_ASSEMBLY);
}

void Matrix::Print()
{
    Message::Debug("Size of Matrix: %d %d", m_row, m_col);
    //MatView(m_mat, PETSC_VIEWER_STDOUT_WORLD);
}
