#include <iostream>
#include <vector>
#if defined(HAVE_MPI) && defined(HAVE_PETSC)
#include <mpi.h>
#include <petsc.h>

#include <Common/Message.h>
#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>

#include <Core/Operator.h>
#include <BIO/BIOUtils.h>

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
  Message::Barrier();

  //Test Circle
  double r1 = 1;
  double x1 = 0.5, y1 = 0.1;
  Circle c1(r1, x1, y1);
  c1.Print();

  //Test intersection
  double r2 = 1;
  double x2 = 10.5, y2 = 0.1;
  Circle c2(r2, x2, y2);
  c2.Print();
  Message::Info("Intersection ? %d", static_cast<int>(c1.Intersect(&c2)));
  //

  // Test Operator
  Operator op;
  op.Print();

  Operator A(2, 2);
  A.Print();
  Operator AA(4, 4);
  AA.Print();

  Message::Warning("== Constructor that call a constructor ==");
  Coord rc;
  rc.nrow = 2; rc.ncol = 4;
  Operator B(rc);
  B.Print();
  Message::Info("==== Failed! expected: shape:2x4 ; Shape:0x0  :-(");


  Operator C(4, 4);
  C.Print();

  Message::Warning(" == Start addBlock ==");

  Message::Info("add A");
  op.addBlock(0, 0, &A);
  op.Print();

  Message::Info("add AA (error?)");
  op.addBlock(0, 0, &AA);
  op.Print();

  Message::Info("add B");
  op.addBlock(0, 1, &B);
  op.Print();

  Message::Info("add C");
  op.addBlock(1, 1, &C);
  op.Print();

  //

  Message::Warning(" == Start BIO ==");
  BIO bio(3, 3);
  BIOVal biov(2, 2, 2);
  BIOZero zero(4, 4);
  BIOEye eye(5, 5);

  Operator a = bio.create();
  Operator b = biov.create();
  Operator c = zero.create();
  Operator d = eye.create();

  a.Print();
  
  Operator fromOp;
  fromOp.addBlock(0, 0, &a);
  fromOp.addBlock(1, 1, &b);
  fromOp.addBlock(2, 2, &c);
  fromOp.addBlock(3, 3, &d);
  fromOp.Print();

  Operator fromBIO;
  fromBIO.addBlock(0, 0, &bio);
  fromBIO.addBlock(1, 1, &biov);
  fromBIO.addBlock(2, 2, &zero);
  fromBIO.addBlock(3, 3, &eye);
  fromBIO.Print();
  fromBIO.addBlock(2, 2, &c);
  fromBIO.addBlock(0, 0, &eye);
  

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
