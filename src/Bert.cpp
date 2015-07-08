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
    Operator opAplusB;
    Operator opA, opB;/*, opC, opD;
    Operator opAB, opCD;
    opAB.setBlockSize(2,2);
    opAB.setBlock(0,0,&opA);
    opAB.setBlock(1,1,&opB);
    opAB.Print();
    opCD.setBlockSize(2,2);
    opCD.setBlock(0,0,&opC);
    opCD.setBlock(1,1,&opD);
    opCD.Print();
    Operator TheOp;
    TheOp.setBlockSize(2,2);
    TheOp.setBlock(0, 0, &opAB);
    TheOp.setBlock(1, 1, &opCD);
    TheOp.Print();
    opA.PrintShape();
    opAB.PrintShape();
    TheOp.PrintShape();
		      */
    Message::Debug("--------");
    opAplusB = opA + opB;
    Message::Debug("--------");
    opAplusB.Print();
/*
    Message::Info("Checking...");
    bool checkAandB = opA.checkSize(opB);
    Message::Info("checkAandB = %d", checkAandB);
    bool checkAandAB = opA.checkSize(opAB);
    Message::Info("checkAandAB = %d", checkAandAB);
    bool checkABandCD = opCD.checkSize(opAB);
    Message::Info("checkABandCD = %d", checkABandCD);
    bool checkTheOpandAB = TheOp.checkSize(opAB);
    Message::Info("checkTheOpandAB = %d", checkTheOpandAB);
    bool checkAandAplusB = opA.checkSize(opAplusB);
    Message::Info("checkAandAplusB = %d", checkAandAplusB);
*/
//    Message::Info("thetrace has %d Traces", thetrace.get_number_of_Trace());
    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
