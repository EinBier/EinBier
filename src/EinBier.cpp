#include <iostream>
#include <vector>

#include <Common/Message.h>
#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>

#include <BIO/BIO.h>
#include <BIO/BIOUtils.h>

#include <Core/Node.h>
#include <Core/Operator.h>
#include <Core/Matrix.h>

#if defined(HAVE_MPI) && defined(HAVE_PETSC)
#include <mpi.h>
#include <petsc.h>
#endif

//static char help[] = "Appends to an ASCII file.\n\n";

int main(int argc, char *argv[])
{
    Message::Info("------- Begin");
    Message::Initialize(argc, argv);

#if defined(HAVE_MPI) && defined(HAVE_PETSC)
    PetscErrorCode ierr;
    Vec v;
    int mpirank = Message::GetRank();
    int mpisize = Message::GetNProc();
    int m = 10;
    int m_loc = m/mpisize;
    int m_start = m_loc * mpirank;
    if (mpirank < m % mpisize) {
        m_loc ++;
        m_start += mpirank;
    }
    else
        m_start += m % mpisize;

    int m_end = m_start + m_loc;

    std::vector<PetscScalar> machin(m);
    std::vector<PetscInt> ind(m);
    for (int i = 0; i < m; i++) {
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
#endif

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



    Message::Info("");
    Message::Info("\n");
    Message::Debug(" == Start addBlock ==");

    //// to prevent a circular dependency
    BIO tmp;
    //// otherwise the linker failed:  undefined reference to `BIO::create()'
    //// note: the circular dependency with forward declaration is well-known
    //// and it could be a problem, source of bug ?
    //// It is claimed that the mutual dependency is done by unexperimented programmer.

    Operator op;
    //op.Print();
    Operator A(5, 5);
    //A.Print();
    Operator AA(4, 4);
    //AA.Print();
    Coord rc(5, 7);
    Operator B(rc);
    Operator C(Coord(2, 2));
    //C.Print();

    Message::Info("add A");
    op.addBlock(0, 0, &A);
    op.Print();

    Message::Info("add AA (error?)");
    op.addBlock(0, 0, &AA);
    //op.Print();

    Message::Info("add B");
    op.addBlock(0, 1, &B);
    //op.Print();

    Message::Info("add C");
    op.addBlock(1, 1, &C);
    //op.Print();

    //

    Message::Debug(" == Start BIO and first Operator Assembling ==");
    BIO bio(3, 3);
    BIOVal biov(2, 2, 2);
    BIOZero zero(4, 4);
    BIOEye eye(5, 5);

    bio.Print();
    eye.Print();

    Operator fromBIO;
    Coord coord(0,0);
    fromBIO.addBlock(coord, &bio);
    fromBIO.addBlock(Coord(1,1), &biov);
    fromBIO.addBlock(2, 2, &zero);
    fromBIO.addBlock(3, 3, &eye);
    fromBIO.Print();
    fromBIO.addBlock(Coord(0, 0), &eye);

    Message::Debug(" == Start Matrix ==");
    Matrix myMat(Coord(3, 3));
    int i, j;
    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) {
            myMat.insert(i, j, -1);
        }
    }
    myMat.Print();

    Matrix M = eye.assemb();
	M.Print();
    BIOVal two(5, 5, 2);
    Matrix N = two.assemb();
    //N.Print();
    Matrix Add = M + N;
    //Add.Print();
    Matrix Min = -Add;
    //Min.Print();
    Matrix Diff = M - N;
    //Diff.Print();
    double t = 5;
    Matrix MulSca = Diff*t; // ya precedence sucks!
    MulSca.Print();

    Message::Debug(" == Start Operator Assembling ==");
    Operator Two = two;
    Message::Info("TrueOp + TrueOp");
    Operator zz = Two + Two;
    zz.Print();
    Matrix ZZ = zz.assemb();
    ZZ.Print();
    Message::Info("res-add1 + TrueOp");
    Operator yy = zz + Two;
    yy.Print();
    Matrix YY = yy.assemb();
    YY.Print();
    Message::Info("res-add1 + res-add2");
    Operator xx = zz + yy;
    xx.Print();
    Matrix XX = xx.assemb();
    XX.Print();
    Message::Info("TrueOp * double");
    Operator zzz = Two * 21.;
    zzz.Print();
    Matrix ZZZ = zzz.assemb();
    ZZZ.Print();

    Message::Debug(" == Bug Operator Assembling ==");
    Message::Info("TrueOp + TrueOp + TrueOp");
    //Operator ww = (Two + Two) + Two;
    Operator ww;
    ww = (Two + Two) + Two;
    ww.Print();
    Matrix WW = ww.assemb();
    WW.Print();

    Message::Info("res-mul1 * double");
    Operator yyy = zzz * 1.;
    yyy.Print();
    Matrix YYY = yyy.assemb();
    YYY.Print();

//  Matrix moMulAdd = (((eye.create() + two.create())*2) + two.create()).assemb();
//  moMulAdd.Print();

    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
