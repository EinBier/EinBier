#include <iostream>
#include <vector>

#include <Common/Message.h>

#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>

#include <Trace/Trace.h>

// #include <Operator/Barman.h>
// #include <Operator/Operator.h>


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

    Trace a("a");

    Trace u("u");
    Trace v("v");
    Trace w("w");

    Trace P("P");
    P += &u;
    P += &v;
    // P.push_back(&u);
    // P.push_back(&v);

    P.push_back(&P);

    Trace *V = P[1];
    V->Print();

    Trace Q("Q");
    Q.push_back(&v);
    Q.push_back(&w);

    Trace L("L");
    L.push_back(&P);
    L.push_back(&w);

    Trace M("M");
    M.push_back(&u);
    M.push_back(&Q);

    Trace LL("LL");
    LL.push_back(&L);
    LL.push_back(&a);

    Trace MM("MM");
    MM.push_back(&M);
    MM.push_back(&a);

    Message::Debug("u");
    Trace UU = u.flatize();
    UU.Print();

    Message::Debug("Deb");


    LL.Print();

    std::cout << std::endl;
    Trace ll = LL.flatize();

    std::cout << std::endl;

    Trace mm = MM.flatize();
    std::cout << std::endl;
    LL.Print();
    Message::Debug("kikool!!");
    ll.Print();
    Message::Debug("end!!");

    mm.Print();

    std::cout << std::endl;
    std::cout << "2= " << ll.compare(&mm) << std::endl;
    std::cout << "3= " << LL.compare(&MM) << std::endl;


    Trace R("R");
    R += &LL;
    R += &u;
    R += &MM;
    R.Print();
    Trace rr = R.flatize();
    rr.Print();




//     TraceElem d("D");
//     TraceElem e("E");
//     TraceElem f("F");
//     Trace ab;
//     ab.push_back(&a);
//     ab.push_back(&b);
//     Trace cd;
//     cd.push_back(&c);
//     cd.push_back(&d);
//     Trace abcd;
//     abcd.push_back(&ab);
//     abcd.push_back(&cd);
//     Trace thetrace;
//     thetrace.push_back(&abcd);
//     thetrace.push_back(&e);
//     thetrace.Print();
// //Operators
//     Operator opAplusB;
//     Operator opA, opB;/*, opC, opD;
//     Operator opAB, opCD;
//     opAB.setBlockSize(2,2);
//     opAB.setBlock(0,0,&opA);
//     opAB.setBlock(1,1,&opB);
//     opAB.Print();
//     opCD.setBlockSize(2,2);
//     opCD.setBlock(0,0,&opC);
//     opCD.setBlock(1,1,&opD);
//     opCD.Print();
//     Operator TheOp;
//     TheOp.setBlockSize(2,2);
//     TheOp.setBlock(0, 0, &opAB);
//     TheOp.setBlock(1, 1, &opCD);
//     TheOp.Print();
//     opA.PrintShape();
//     opAB.PrintShape();
//     TheOp.PrintShape();
// 		      */
//     Message::Debug("--------");
//     opAplusB = opA + opB;
//     Message::Debug("--------");
// //    opAplusB.Print();
// /*
//     Message::Info("Checking...");
//     bool checkAandB = opA.checkSize(opB);
//     Message::Info("checkAandB = %d", checkAandB);
//     bool checkAandAB = opA.checkSize(opAB);
//     Message::Info("checkAandAB = %d", checkAandAB);
//     bool checkABandCD = opCD.checkSize(opAB);
//     Message::Info("checkABandCD = %d", checkABandCD);
//     bool checkTheOpandAB = TheOp.checkSize(opAB);
//     Message::Info("checkTheOpandAB = %d", checkTheOpandAB);
//     bool checkAandAplusB = opA.checkSize(opAplusB);
//     Message::Info("checkAandAplusB = %d", checkAandAplusB);
// */
//    Message::Info("thetrace has %d Traces", thetrace.get_number_of_Trace());
//    Barman::Clear();
    Message::InfoRoot("End-------");
    return Message::Finalize(EXIT_SUCCESS);

}
