#include <iostream>
#include <vector>

#include <Common/Message.h>
#include <Common/Type.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>


#include <Core/Operator.h>
#include <Core/OperatorUtils.h>
#include <Core/Matrix.h>
#include <Core/Barman.h>
#include <Core/BIO.h>

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
//Test de l'operatorHandler
    Message::Debug(" == Init Barman ==");
    Barman *opHand = NULL;
    Barman::Init();
    Message::Debug("Barman %d", opHand);
    opHand=opHand->getBarman();
    Message::Info("opHand %d", opHand);

    Message::Debug("-----------------------------");
    Operator A (2,2,false), B(2,2,false);
    Operator C;
    C=A+B;
    Message::Debug("OK");

    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
