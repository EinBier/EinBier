#include <iostream>
#include <vector>
#if defined(HAVE_MPI) && defined(HAVE_PETSC)
#include <mpi.h>
#include <petsc.h>

#include <Common/Message.h>

//static char help[] = "Appends to an ASCII file.\n\n";

int main(int argc, char *argv[])
{
  Message::Info("------- Begin");
  Message::Initialize(argc, argv);
  
  PetscErrorCode ierr;
  Vec v;
  int mpirank = Message::GetRank();
  int mpisize = Message::GetNProc();
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

  Message::Info("MPI rank %d, m_start = %d, m_loc =%d", mpirank,m_start, m_loc);

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

  Message::InfoRoot("End-------");
  Message::Finalize(EXIT_SUCCESS);
  return 0;
}

#else
int main(int argc, char *argv[])
{
  std::cout << "Without PETSC and/or MPI, you will see nothing, bro..." << std::endl;
  return 0;
}
#endif
