#include <iostream>
#include <vector>

#include <Common/Message.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>


#if defined(HAVE_MPI)
#include <mpi.h>
#endif

#if defined(HAVE_PETSC)
#include <petsc.h>
#endif

//static char help[] = "Appends to an ASCII file.\n\n";

int main(int argc, char *argv[])
{
    Message::Info("------- Begin EinBier");
    Message::Initialize(argc, argv);
    int mpirank = Message::GetRank();
    int mpisize = Message::GetNProc();

#if defined(HAVE_PETSC)
    PetscErrorCode ierr;
#endif
//Test de l'operatorHandler
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
