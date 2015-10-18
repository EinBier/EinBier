#include<Common/Message.h>
#include <Common/Shape.h>

#include <Trace/Trace.h>

#include <Operator/BierOperator.h>

#include <iostream>

BierOperator::BierOperator(std::string name) {

    m_name = name,

    m_test = nullptr;
    m_trial = nullptr;

    m_del_test = false;
    m_del_trial = false;

    m_offset_test.resize(1, 0);
    m_offset_trial.resize(1, 0);

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

    if(m_test != nullptr)
        m_test->Print();
    if(m_trial != nullptr)
        m_trial->Print();
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
