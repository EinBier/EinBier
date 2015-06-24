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
    Message::Debug(" == Init OperatorHandler ==");
    OperatorHandler *opHand = NULL;
    OperatorHandler::Init();
    Message::Debug("OperatorHandler %d", opHand);
    opHand=opHand->getOperatorHandler();
    Message::Info("opHand %d", opHand);

    Message::Debug(" == Start BIO and first Operator Assembling ==");
    Message::Debug("Build bio");
    BIO bio(2, 2);
    Message::Debug("Print bio");
    bio.Print();
    Operator A, B(2,2), C(2,2);
    Message::Debug("Somme A = B + C");
    A = B + C;
    Message::Debug("OK");

    OperatorHandler::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
