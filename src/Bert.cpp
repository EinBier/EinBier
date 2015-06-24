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
    Message::Info("------- Begin BERT");
    Message::Initialize(argc, argv);

#if defined(HAVE_MPI) && defined(HAVE_PETSC)
    PetscErrorCode ierr;
    int mpirank = Message::GetRank();
    int mpisize = Message::GetNProc();

    Message::Info("MPI rank %d, m_start = %d, m_loc =%d", mpirank,m_start, m_loc);
    // Test class
    Message::Barrier();
#endif
//Test de l'operatorHandler
    Message::Debug(" == Init OperatorHandler ==");
    OperatorHandler *opHand = NULL;
    OperatorHandler::createOperatorHandler();
    Message::Debug("OperatorHandler %d", opHand);
    opHand=opHand->getOperatorHandler();
    Message::Info("opHand %d", opHand);

    Message::Debug(" == Start BIO and first Operator Assembling ==");
    Message::Debug("Build bio");
    BIO bio(2, 2);
    Message::Debug("Print bio");
    bio.Print();
    Message::Debug("Build A");
    Operator A(2,2);//, B(2,2), C(2,2);
//    A = B+C;
    Message::Debug("Print A");
    A.Print();
    Message::Debug("Create B");
    Operator B;
    Message::Debug("Print B");
    B.Print();
    B = A;
    Message::Debug("Print B (after B = A)");
    B.Print();

    opHand->Print();
    if(opHand != NULL)
	delete opHand;
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
