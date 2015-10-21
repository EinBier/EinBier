#include <Common/Message.h>
#include <Barman/Barman.h>
#include <Operator/BierOperator.h>
#include <Operator/Operator.h>




Operator::Operator(std::string name)
{
    BierOperator *res = new BierOperator(name, true);
    m_id = res->getId();
    Message::Debug("Operator %s {aka %d} (initialized)", name.c_str(), m_id);
}

Operator::~Operator() {
    Message::Debug("Operator %d (destroyed)", m_id);
}

BierOperator* Operator::getBierOp() {
    BierOperator *b;
    b = dynamic_cast<BierOperator*>(Barman::get_Bier_ptr(m_id));
    if (b == nullptr) {
        Message::Warning("Empty Bier.");
    }
    return b;
}

void Operator::Print() {
    BierOperator *b = getBierOp();
    if (b != nullptr) {
        Message::Info("Operator: %d", m_id);
        b->Print();
    } else {
        Message::Info("Operator %d is empty.", m_id);
    }
}

void Operator::setTrace(Trace* u, Trace* v) {
    BierOperator *b = getBierOp();
    b->setTrace(u, v);
}

void Operator::setBlock(int i, int j, Operator& A) {
    BierOperator *b = getBierOp();
    BierOperator *a = A.getBierOp();
    b->setBlock(i, j, a);
}

Operator Operator::operator+() {
    Operator B;
    BierOperator *b = B.getBierOp();
    b->getNode()->set("+", m_id);
    BierOperator *a = getBierOp();
    b->setTrace(a->getTraceTest(), a->getTraceTrial());
    return  B;
}

Operator Operator::operator-() {
    Operator B;
    BierOperator *b = B.getBierOp();
    b->getNode()->set("-", m_id);
    BierOperator *a = getBierOp();
    b->setTrace(a->getTraceTest(), a->getTraceTrial());
    return  B;
}

Operator Operator::operator*(double scalar) {
    Operator B;
    BierOperator *b = B.getBierOp();
    BierOperator *a = getBierOp();
    b->getNode()->set(scalar, a->getId());
    b->setTrace(a->getTraceTest(), a->getTraceTrial());
    return  B;
}

Operator operator*(double scalar, Operator A) {
    return A * scalar;
}

Operator operator+(Operator A, Operator B) {
    Operator C;

    BierOperator *a = A.getBierOp();
    BierOperator *b = B.getBierOp();
    BierOperator *c = C.getBierOp();

    if (*(a->getTraceTest()) != b->getTraceTest()
        || *(a->getTraceTrial()) != b->getTraceTrial()) {
        Message::Error("Incompatible Traces");
        return C;
    }

    c->getNode()->set(a->getId(), "+", b->getId());
    c->setTrace(a->getTraceTest(), a->getTraceTrial());
    return  C;
}

Operator operator-(Operator A, Operator B) {
    Operator C;

    BierOperator *a = A.getBierOp();
    BierOperator *b = B.getBierOp();
    BierOperator *c = C.getBierOp();

    if (*(a->getTraceTest()) != b->getTraceTest()
        || *(a->getTraceTrial()) != b->getTraceTrial()) {
        Message::Error("Incompatible Traces");
        return C;
    }

    c->getNode()->set(a->getId(), "-", b->getId());
    c->setTrace(a->getTraceTest(), a->getTraceTrial());
    return  C;
}
