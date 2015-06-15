#include <iostream>
#include <vector>
#ifdef HAVE_MPI && HAVE_PETSC
#include <mpi.h>
#include <petscvec.h>

//static char help[] = "Appends to an ASCII file.\n\n";

int main(int argc, char *argv[])
{
  std::cout << "------- Begin" << std::endl;
  //  MPI_Init(&argc, &argv);
  PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
  int mpirank, mpisize;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
  
  std::cout << "MPI rank " << mpirank << " / " << (mpisize-1) << std::endl;

    PetscErrorCode ierr;
  Vec v;
  int m = 10;
  int m_loc = m/mpisize;
  int m_start = m_loc * mpirank;
  if( mpirank < m % mpisize)
    {
      m_loc ++;
      m_start += mpirank;
    }
  else
    m_start += m % mpisize;

  int m_end = m_start + m_loc;

  std::vector<PetscScalar> machin(m);
  std::vector<PetscInt> ind(m);
  for (int i = 0; i < m; i++)
    {
      machin[i] = static_cast<PetscScalar>(i + 1.2);
      ind[i] = static_cast<PetscInt>(i);
    }

  std::cout << "MPI rank " << mpirank << "; m_start = " << m_start << " and m_loc= "<< m_loc<< std::endl;

  VecCreate(PETSC_COMM_WORLD, &v);
  VecSetSizes(v, m_loc, m);
  VecSetFromOptions(v);
  VecGetOwnershipRange(v, &m_start, &m_end);
  VecSetValues(v, m_loc, &ind[m_start], &machin[m_start], INSERT_VALUES);
  VecAssemblyBegin(v);
  VecAssemblyEnd(v);
  VecView(v, PETSC_VIEWER_STDOUT_SELF);
  VecDestroy(&v);

  // Test class
  MPI_Barrier(MPI_COMM_WORLD);


  PetscFinalize();
  
  std::cout << "End-------" << std::endl;
  return 0;
}

#else
int main(int argc, char *argv[])
{
  std::cout << "Without PETSC and/or MPI, you will see nothing, bro..." << std::endl;
  return 0;
}
#endif
