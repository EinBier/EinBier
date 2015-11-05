#include <iostream>
#include <vector>

#include <Common/Message.h>


#include <Barman/Barman.h>
#include <Barman/Bier.h>

#include <Operator/BierOperator.h>
#include <Trace/Trace.h>
#include <Operator/Operator.h>
#include <Matrix/Matrix.h>

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

    Matrix A;
    int row = 4;
    int col = 6;
    A.setSize(row, col);
    // for (int i = 0; i < row; i++)
    // 	A.setValue(i, i, 1.);
    // A.assemble();
    A.Print();
    Message::Info("###END");
    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
