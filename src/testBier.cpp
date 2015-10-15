#include <iostream>
#include <vector>

#include <Common/Message.h>


#include <Barman/Barman.h>
#include <Barman/Bier.h>

#include <Operator/BierOperator.h>
#include <Trace/Trace.h>

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


    Barman::Init();
    Barman::Print();

    Bier b;
    Barman::Print();

    Bier a, c, d;
    Barman::Print();

    Bier *A;
    A = new Bier(true);
    Barman::Print();

    Barman::Clear();

    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
