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

    Message::setDebug();
    Barman::Init();

    BierOperator A("A");
    A.Print();
    Message::Info("--");

    Trace u("u"), v("v");
    A.setTrace(&u, &v);
    Message::Info("<A u, v>");
    A.Print();

    Trace w("w");
    w.push_back(&u);
    w.push_back(&v);

    BierOperator B("B");
    B.setTrace(&w, &w);
    Message::Info("<B w, w> with w = [u, v]");
    B.Print();

    BierOperator C("C");
    C.setBlock(0,0,&A);
    C.setBlock(1,1,&B);
    Message::Info("C = [A, 0; 0, B] ");
    C.Print();

    C.setBlock(0,1,&A);


    Message::Info("###END");

    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
