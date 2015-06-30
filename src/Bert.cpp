#include <iostream>
#include <vector>

#include <Common/Message.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>

#include <Trace/Trace.h>

#include <Operator/Barman.h>
#include <Operator/Operator.h>


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
    TraceElem a("A");
    TraceElem b("B");
    TraceElem c("C");
    TraceElem d("D");
    TraceElem e("E");
    TraceElem f("F");
    a.Print();
    Trace ab;
    ab.push_back(&a);
    ab.push_back(&b);
    Trace cd;
    cd.push_back(&c);
    cd.push_back(&d);
    Trace abcd;
    abcd.push_back(&ab);
    abcd.push_back(&cd);
    Trace thetrace;
    thetrace.push_back(&abcd);
    thetrace.push_back(&e);
    thetrace.Print();
//Operators
    Operator opA, opB, opC, opD;
    Operator TheOp;
    TheOp.setBlockSize(2,2);
    TheOp.setBlock(0, 0, &opA);
    TheOp.setBlock(0, 1, &opB);
    TheOp.setBlock(1, 0, &opC);
    TheOp.setBlock(1, 1, &opD);
    TheOp.Print();
//    Message::Info("thetrace has %d Traces", thetrace.get_number_of_Trace());

//Test de l'operatorHandler
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
