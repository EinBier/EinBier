#include <iostream>

#include<Common/Message.h>
#include <Common/Shape.h>

#include <Trace/Trace.h>

#include <Operator/BierOperator.h>

#include <Matrix/Matrix.h>
#include <Barman/Barman.h>

//#include <petsc.h>

std::string BierOperator::ELEMENTARY = "elementary";
std::string BierOperator::BLOCK = "block";
std::string BierOperator::BELEM = "belem";
std::string BierOperator::UNDEFINED = "undefined";

std::string BierOperator::UNARY = "unary";
std::string BierOperator::BINARY = "binary";


BierOperator::BierOperator(std::string name, bool in_charge) : Bier(in_charge) {

    m_name = name,

    m_test = nullptr;
    m_trial = nullptr;

    m_del_test = false;
    m_del_trial = false;

    m_offset_test.resize(1, 0);
    m_offset_trial.resize(1, 0);

    m_mat = nullptr;
    m_isassemble = false;

    Message::Debug("BierOperator %s {aka: %d} (initialized)", m_name.c_str(), m_id);
}

BierOperator::~BierOperator() {
    if (m_del_test) {
        delete m_test;
        Message::Debug("BierOperator %s Trace test deleted", m_name.c_str());
    }
    if (m_del_trial) {
        delete m_trial;
        Message::Debug("BierOperator %s Trace trial deleted", m_name.c_str());
    }
    if (m_mat != nullptr) {
        delete m_mat;
        Message::Debug("BierOperator %s Matrix deleted", m_name.c_str());
    }
    Message::Debug("BierOperator %s (destroyed)", m_name.c_str());
}


void BierOperator::setTrace(Trace* test, Trace* trial) {
    if (test == nullptr) {
        Message::Error("Trace Test is not defined.");
    }
    if (trial == nullptr) {
        Message::Error("Trace Trial is not defined.");
    }

    m_offset_test.resize(2);
    if (test->isBlock()) {
        m_test = new Trace("test");
        m_del_test = true;
        *m_test = test->flatize();
        // NOTE:
        //   m_test->getSize() calls flatize() then getNumberOfTrace()
        //   therefore, let avoid an unnecessary call to flatize()
        //   a bit less consuming
        m_offset_test[1] = m_test->getNumberOfTrace();
    } else {
        m_test = test;
        // NOTE: getSize() returns 1 without doing more
        m_offset_test[1] = m_test->getSize();
    }

    m_offset_trial.resize(2);
    if (trial->isBlock()) {
        m_trial = new Trace("trial");
        m_del_trial = true;
        *m_trial = trial->flatize();
        m_offset_trial[1] = m_trial->getNumberOfTrace();
    } else {
        m_trial = trial;
        m_offset_trial[1] = m_trial->getSize();
    }

    // type = BELEM
}

void BierOperator::Print() {
    Shape size = getSize();

    Message::Info("BierOperator: %s {aka: %d}", m_name.c_str(), m_id);
    Message::Info("       Size : %d x %d", size[0], size[1]);
    Message::Info("      Shape : %d x %d", m_Shape[0], m_Shape[1]);
    Message::Info("       Type : %s", m_node.getType().c_str());

    if(m_test != nullptr)
        m_test->Print();
    if(m_trial != nullptr)
        m_trial->Print();

    if (m_node.getType() == BierOperator::BLOCK) {
        for (int i=0; i<m_Shape[0]; i++) {
            Message::Print(" [ ");
            for (int j=0; j<m_Shape[1]; j++) {
                int id = m_node.getBlock(i, j);
                Message::Print(0, " %d ", id);
            }
            Message::Print(0, " ] \n");
        }
    }
}


Shape BierOperator::getSize() {
    int r=0 , c=0;
    if (m_test != nullptr) {
        int L = m_offset_test.size() - 1;
        r = m_offset_test[L];
    }
    if (m_trial != nullptr) {
        int L = m_offset_trial.size() - 1;
        c = m_offset_trial[L];
    }
    return Shape(r, c);
}

Shape BierOperator::getShape() {
    return m_Shape;
}

Trace BierOperator::getSubTrace(Trace* t, int i, std::vector<int> offset) {
    Trace tmp("tmp-SubTrace");
    if (i<offset.size()) {
        int ii = offset[i];
        int nelem = offset[i+1] - offset[i];
        for (int jj=ii; jj<ii+nelem; jj++) {
            tmp.push_back(t->getTrace(jj));
        }

    }
    return tmp;
}

void BierOperator::setBlock(int i, int j, BierOperator *A) {
    if (m_test == nullptr) {
        m_test = new Trace("test");
        m_del_test = true;
    }
    if (m_trial == nullptr) {
        m_trial = new Trace("trial");
        m_del_trial = true;
    }

    Shape s = A->getSize();

    Trace test = getSubTrace(m_test, s[0], m_offset_test);
    Trace trial = getSubTrace(m_trial, s[1], m_offset_trial);

    bool check_test = true;
    bool check_trial = true;

    if (test.isBlock()) {
        if (test.compare(A->getTraceTest()) == 0)
            check_test = false;
    }
    if (trial.isBlock()) {
        if (trial.compare(A->getTraceTrial()) == 0)
        check_trial = false;
    }

    if (!check_test || !check_trial) {
        Message::Error("Block badly sized.");
        int r = test.getNumberOfTrace();
        int c = trial.getNumberOfTrace();
        Message::Error(" size(Input)= %dx%d != %dx%d =size(Acceptable)",
                       s[0], s[1], r, c);
        return;
    }

    // Block accepted !!

    m_node.set(i, j, A->getId());

    if (i >= m_offset_test.size()-1) {
        int L = m_offset_test.size() - 1;
        m_offset_test.resize(i+2, m_offset_test[L]);

        int r = A->getSize()[0];
        L = m_offset_test.size() - 1;
        for(int k=i+1; k<=L; k++)
            m_offset_test[k] += r;
        m_Shape.set_row(i+1);
    }
    if (j >= m_offset_trial.size()-1) {
        int L = m_offset_trial.size() - 1;
        m_offset_trial.resize(i+2, m_offset_trial[L]);

        int r = A->getSize()[0];
        L = m_offset_trial.size() - 1;
        for(int k=i+1; k<=L; k++)
            m_offset_trial[k] += r;
        m_Shape.set_col(i+1);
    }

    Shape sizeA = A->getSize();

    Trace *testA = A->getTraceTest();
    if (testA->isBlock()) {
        for (int k=sizeA[0]-1; k>=0; k--) {
            m_test->insert(m_offset_test[i], testA->getTrace(k));
        }
    } else {
        m_test->insert(m_offset_test[i], testA);
    }

    Trace *trialA = A->getTraceTrial();
    if (trialA->isBlock()) {
        for (int k=sizeA[1]-1; k>=0; k--) {
            m_trial->insert(m_offset_trial[i], trialA->getTrace(k));
        }
    } else {
        m_trial->insert(m_offset_trial[i], trialA);
    }
}





///////////////////////////////////////////
// ************************************* //
///////////////////////////////////////////

BNode::BNode() {
    reset();
}

void BNode::reset() {
    m_type = BierOperator::UNDEFINED;

    m_operation = "";
    m_OpIdR = -1;
    m_OpIdL = -1;

    m_scalar = 0.;

    m_OpIds.resize(0);
    m_indices.resize(0);
}

void  BNode::setElementary()
{
    m_type = BierOperator::ELEMENTARY;
}


// Binary: A+B or A-B
void BNode::set(int OpIdL, std::string operation, int OpIdR) {
    if (operation != "+" && operation != "-") {
        Message::Error("Operation not supported.");
        return;
    }
    reset();
    m_type =  BierOperator::BINARY;
    m_operation = operation;
    m_OpIdL = OpIdL;
    m_OpIdR = OpIdR;
}

// Scalar: 2*A or A*2
void BNode::set(double scalar, int OpIdR) {
    reset();
    m_type =  BierOperator::UNARY;
    m_operation = "*";
    m_OpIdR = OpIdR;
    m_scalar = scalar;
}


// Unary: -A or +A
void BNode::set(std::string operation, int OpId) {
    if (operation != "+" && operation != "-") {
        Message::Error("Operation not supported.");
        return;
    }
    reset();
    m_type =  BierOperator::UNARY;
    m_operation = operation;
    m_OpIdR = OpId;
}

// Block
void BNode::set(int row, int col, int OpId) {
    if (m_type !=  BierOperator::UNDEFINED && m_type !=  BierOperator::BLOCK) {
        Message::Error("Incompatible type.");
        return;
    }
    m_type =  BierOperator::BLOCK;
    m_OpIds.push_back(OpId);
    m_indices.push_back(Shape(row, col));
}

int BNode::getBlock(int i, int j) {
    Shape ij(i, j);
    for (int ii=0; ii<m_OpIds.size(); ii++) {
        if (ij == m_indices[ii])
            return m_OpIds[ii];
    }
    return -1;
}

Scalar BierOperator::getValue(int i, int j)
{
    // i, j are local to this operator

    if (m_mat != nullptr && m_isassemble)
        return m_mat->getValue(i, j);

    BNode *node = getNode();
    std::string type = node->getType();

    Message::Debug("BierOperator %s getValue type:%s", m_name.c_str(), type.c_str());

    if (type == BierOperator::ELEMENTARY) {
        Message::Debug("Computing %s", m_name.c_str());
        return m_trial->getValue(m_k, i, j, m_operator, m_test);
    } else if (type == BierOperator::UNARY) {

        std::string op = node->getOperation();
        BierOperator *A = dynamic_cast<BierOperator*> (Barman::get_Bier_ptr(node->getOpIdR()));

        if (op == "+")
            return A->getValue(i, j);
         else if (op == "-")
             return -A->getValue(i, j);
         else if (op == "*")
             return node->getScalar() * A->getValue(i, j);


    } else if (type == BierOperator::BINARY) {

        std::string op = node->getOperation();
        BierOperator *A = dynamic_cast<BierOperator*> (Barman::get_Bier_ptr(node->getOpIdR()));
        BierOperator *B = dynamic_cast<BierOperator*> (Barman::get_Bier_ptr(node->getOpIdL()));

        if (op == "+")
            return A->getValue(i, j) + B->getValue(i, j);
         else if (op == "-")
            return A->getValue(i, j) - B->getValue(i, j);


    } else
        return 0.;
}

void BierOperator::assemble()
{

    Message::Debug("BierOperator %s assembling...", m_name.c_str());
    int Row = 2;
    Int Col = 2;
    m_mat = new Matrix;
    m_mat->setSize(Row, Col);
    for (int i=0; i<Row; i++) {
        for (int j=0; j<Col; j++) {
            Scalar d = getValue(i, j);
            m_mat->setValue(i, j, d);
        }
    }
    m_mat->assemble();
    m_isassemble = true;
    Message::Debug("BierOperator %s assembled", m_name.c_str());
    m_mat->Print();
}

void BierOperator::setOperator(std::string op, Scalar k)
{
    // TODO: check ?
    m_k = k;
    m_operator = op;
}

Scalar compute_tmp(int i, int j)
{
    if (i == j)
        return 1.;
    else
        return 0.;
}

