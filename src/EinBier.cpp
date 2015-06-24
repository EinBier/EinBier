#include <iostream>
#include <vector>

#include <Common/Message.h>
#include <Common/Type.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>


#include <Core/Operator.h>
#include <Core/OperatorUtils.h>
#include <Core/Matrix.h>
#include <Core/OperatorHandler.h>
#include <Core/BIO.h>

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
    Message::Debug(" == Start setBlock ==");

    Operator op;
    op.Print();
    Operator A(5, 5);
    A.Print();

    Operator AA(4, 4);
    //AA.Print();
    Shape rc(5, 7);
    Operator B(rc);
    Operator C(Shape(2, 2));
    //C.Print();

    Message::Info("add A");
    op.setBlock(0, 0, &A);
    op.Print();

    Message::Info("add AA (error?)");
    op.setBlock(0, 0, &AA);
    //op.Print();

    Message::Info("add B");
    op.setBlock(0, 1, &B);
    //op.Print();

    Message::Info("add C");
    op.setBlock(1, 1, &C);
    //op.Print();

    //

    Message::Debug(" == Start BIO and first Operator Assembling ==");
    BIO bio(3, 3);

    OperatorVal biov(2, 2, 2);
    OperatorZero zero(4, 4);
    OperatorEye eye(5, 5);

    bio.Print();
    eye.Print();

    Operator fromBIO;
    Shape coord(0,0);
    fromBIO.setBlock(coord, &bio);
    fromBIO.setBlock(Shape(1,1), &biov);
    fromBIO.setBlock(2, 2, &zero);
    fromBIO.setBlock(3, 3, &eye);
    fromBIO.Print();
    fromBIO.setBlock(Shape(0, 0), &eye);

    Message::Debug(" == Start Matrix ==");
    Matrix myMat(Shape(3, 3));
    int i, j;
    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) {
            myMat.insert(i, j, -1);
        }
    }
    myMat.Print();

    Matrix MM = myMat + myMat;
    MM.Print();

    Matrix M = eye.assemble();
    M.Print();

    OperatorVal two(5, 5, 2);
    Matrix N = two.assemble();
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
    OperatorVal Two(4, 4, 2);
    Message::Info("TrueOp + TrueOp");
    Operator zz = Two + Two;
    zz.Print();
    Matrix ZZ = zz.assemble();
    ZZ.Print();
    Message::Info("res-add1 + TrueOp");
    Operator yy = zz + Two;
    yy.Print();
    Matrix YY = yy.assemble();
    YY.Print();
    Message::Info("res-add1 + res-add2");
    Operator xx = zz + yy;
    xx.Print();
    Matrix XX = xx.assemble();
    XX.Print();
    Message::Info("TrueOp * double");
    Operator zzz = Two * 21.;
    zzz.Print();
    Matrix ZZZ = zzz.assemble();
    ZZZ.Print();

    Message::Info("TrueOp + TrueOp + TrueOp");
    OperatorVal Three(4, 4, 3);
    Operator ww;
    ww = (Three + Three) + Three;
    ww.Print();
    Matrix WW = ww.assemble();
    WW.Print();

    OperatorVal Four(4, 4, 4);
    Operator ww4;
    ww4 = (Two + Four) + Three;
    ww4.Print();
    Matrix WW4 = ww4.assemble();
    WW4.Print();

    OperatorVal Five(4, 4, 5);
    Operator ww5;
    ww5 = (Two + Five) + Three;
    ww5.Print();
    Matrix WW5 = ww5.assemble();
    WW5.Print();

    Message::Info("res-add + res-add + res-add");
    Operator www = ww + ww4 + ww5;
    Matrix WWW = www.assemble();
    WWW.Print();

    Message::Info("(res-add) * double");
    Operator yyy = (www * 2.);
    yyy.Print();
    Matrix YYY = yyy.assemble();
    YYY.Print();

    Message::Debug(" == Bug Operator Assembling ==");

    Operator yyyy = (www * 1.)*2;
    yyyy.Print();
    Matrix YYYY = yyyy.assemble();
    YYYY.Print();
    WWW.Print();

    // Operator xxx = ( ((Two+Two)*1) + (Four*1))*2;
    // Matrix XXX = xxx.assemble();
    // XXX.Print();

//Test de l'operatorHandler
    OperatorHandler *opHand = NULL;
    Message::Debug("OperatorHandler %d [%p]", opHand, opHand);
    opHand=opHand->getOperatorHandler();
    Message::Debug("OperatorHandler %d [%p]", opHand, opHand);
    opHand->removeOperator(bio.get_id());
    opHand->removeOperator(&Two);

    if(opHand != NULL)
        delete opHand;

    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
