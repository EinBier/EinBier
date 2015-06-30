#include <iostream>
#include <vector>

#include <Common/Message.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>

#include <Trace/Trace.h>
#include <Trace/VectorTrace.h>

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
    Trace a("A");
    Trace b("B");
    VectorTrace t1;
    t1.push_back(&a);
    t1.push_back(&b);
    t1.Print();

    Trace c("C");
    Trace d("D");

    VectorTrace t2, t3;
    t2.push_back(&c);
    t3.push_back(&d);

    VectorTrace thet;
    thet.push_back(&t1);
    thet.push_back(&t2);
    thet.push_back(&t3);
    thet.Print();

//Test de l'operatorHandler
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
