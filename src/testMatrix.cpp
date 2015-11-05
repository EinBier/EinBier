#include <iostream>
#include <vector>

#include <Common/Message.h>
#include <Common/Type.h>


#include <Barman/Barman.h>
#include <Barman/Bier.h>

#include <Operator/BierOperator.h>
#include <Trace/Trace.h>
#include <Operator/Operator.h>
#include <Matrix/Matrix.h>

#include <FunctionSpace/FunctionSpace.h>


#if defined(HAVE_MPI)
#include <mpi.h>
#endif

#if defined(HAVE_PETSC)
#include <petsc.h>
#endif

//static char help[] = "Appends to an ASCII file.\n\n";

int main(int argc, char *argv[])
{
    Message::Info("------- Begin BERT");
    Message::Initialize(argc, argv);
    int mpirank = Message::GetRank();
    int mpisize = Message::GetNProc();

#if defined(HAVE_PETSC)
    PetscErrorCode ierr;
#endif

    Message::setDebug();
    Barman::Init();

    // Matrix M;
    // int row = 4;
    // int col = 6;
    // M.setSize(row, col);
    // for (int i = 0; i < row; i++)
    // 	M.setValue(i, i, 1.);
    // M.assemble();
    // M.Print();

    // Scalar d = M.getValue(1, 1);
    // Message::Info("(1,1)= %g", d);
    // d = M.getValue(0, 0);
    // Message::Info("(0,0)= %g", d);


    Operator A("A");
    Operator B("B");
    Operator C("C");

    Dummy fs;

    Trace u;
    Geometry geo;
    u.setGeometry(&geo);
    u.setFunctionSpace(&fs);

    A.setTrace(&u, &u);
    B.setTrace(&u, &u);

    A.setOperator("Id", 0);

    A.assemble();

    // Message::Info("\n--\n");

    // C = A + B + 2*A;
    // C.assemble();

    // // C = (2*A  + B)*3 + A + 2*B;
    // // C.assemble();


    Message::Info("###END");
    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
