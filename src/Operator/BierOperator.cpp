#include<Common/Message.h>
#include <Common/Shape.h>

#include <Trace/Trace.h>

#include <Operator/BierOperator.h>

#include <iostream>

BierOperator::BierOperator() {
    m_test = nullptr;
    m_trial = nullptr;

    m_del_test = false;
    m_del_trial = false;

    m_offset_test.resize(1, 0);
    m_offset_trial.resize(1, 0);
}

BierOperator::~BierOperator() {
    if (m_del_test) {
        delete m_test;
        Message::Info("Delete Test.");
    }
    if (m_del_trial) {
        delete m_trial;
        Message::Info("Delete Trial.");
    }
}


void BierOperator::setTrace(Trace* test, Trace* trial) {
    if (test == nullptr) {
        Message::Error("Trace Test is not defined.");
    }
    if (trial == nullptr) {
        Message::Error("Trace Trial is not defined.");
    }
    if (test->isBlock()) {
        m_test = new Trace("test");
        m_del_test = true;
        *m_test = test->flatize();
    } else {
        m_test = test;
    }

    if (trial->isBlock()) {
        m_trial = new Trace("trial");
        m_del_trial = true;
        *m_trial = trial->flatize();
    } else {
        m_trial = trial;
    }
    // type = BELEM

}

void BierOperator::Print() {
    Message::Info("BierOperator: %d", m_id);
    Message::Info("       Size : %d x %d", getSize().get_row(), getSize().get_col());
    Message::Info("       Shape : %d x %d", m_Shape.get_row(), m_Shape.get_col());
    if(m_test != nullptr)
        m_test->Print();
    if(m_trial != nullptr)
        m_trial->Print();
}


Shape BierOperator::getSize() {
    Shape s;
    if (m_test != nullptr)
        s.set_row(m_test->getSize());
    if (m_trial != nullptr)
        s.set_col(m_trial->getSize());
    return s;
}

Shape BierOperator::getShape() {
    return m_Shape;
}

Trace BierOperator::getSubTrace(Trace* t, int i, std::vector<int> offset) {
    Trace tmp;
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

    Trace test = getSubTrace(m_test, s.get_row(), m_offset_test);
    Trace trial = getSubTrace(m_trial, s.get_col(), m_offset_trial);

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
        return;
    }

    if (i >= m_offset_test.size()-1) {
        m_offset_test.resize(i+1, 0);
        for(int k=i+1; k<m_offset_test.size(); k++)
            m_offset_test[k] += A->getSize().get_row();
    }
    if (j >= m_offset_trial.size()-1) {
        m_offset_trial.resize(i+1, 0);
        for(int k=i+1; k<m_offset_trial.size(); k++)
            m_offset_trial[k] += A->getSize().get_col();
    }

    Trace *testA = A->getTraceTest();
    if (testA->isBlock()) {
        for (int k=A->getSize().get_row()-1; k>0; k--) {
            m_test->insert(m_offset_test[i], testA->getTrace(j));
        }
    } else {
        m_test->insert(m_offset_test[i], testA);
    }
    Trace *trialA = A->getTraceTrial();
    if (trialA->isBlock()) {
        for (int k=A->getSize().get_row()-1; k>0; k--) {
            m_trial->insert(m_offset_trial[i], trialA->getTrace(j));
        }
    } else {
        m_trial->insert(m_offset_trial[i], trialA);
    }

}
