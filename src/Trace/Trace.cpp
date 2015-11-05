#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>
#include<iostream>


Trace::Trace(std::string name)
{
    m_name = name;
    m_geo = nullptr;
    m_traces.resize(0);

    m_method = -1;

    m_fs = nullptr;

    Message::Debug("Trace %s (initialized)", name.c_str());
}

Trace::Trace(std::string name, Geometry* geo) : Trace(name)
{
    setGeometry(geo);
}

Trace::Trace(std::string name, Geometry* geo, int method) : Trace(name, geo)
{
    m_method = method;
}

Trace::~Trace()
{
    Message::Debug("Trace %s (destroyed)", m_name.c_str());
}

bool Trace::isBlock()
{
    if (getNumberOfTrace() > 0)
        return true;
    else
        return false;
}

bool Trace::isDefined()
{
    if (m_geo != nullptr)
        return true;
    else
        return false;
}


std::string Trace::getType()
{
    if (isBlock())
        return "block";
    else if (isDefined())
        return "elementary";
    else
        return "undefined";
}


void Trace::Print(bool isRoot)
{
    if (isRoot) {
        Message::Info("Trace: %s", m_name.c_str());
        Message::Info("    Geometry: %s", m_geo);
        Message::Info("    Method  : %d", m_method);
        Message::Info("    Type    : %s", getType().c_str());
        Message::Print("%s = ", m_name.c_str());
    }
    if (isBlock()) {
        std::cout<<"[";
        for (int i=0; i<getNumberOfTrace(); i ++) {
            getTrace(i)->Print(false);
            if(i < getNumberOfTrace()-1)
                Message::Print(0, ", ");
        }
        Message::Print(0, "]");
    } else {
        if (!isDefined())
            Message::Print(0, "~");
        std::cout << m_name ;
        if (isRoot)
            Message::Print(0, " -- obvious, right ? :-)");
    }
    if (isRoot)
        Message::Print(0, "\n");
}


void Trace::push_back(Trace* t)
{
    if (this == t) {
        Message::Error("Impossible, circular building.");
        return;
    } else if (isDefined()) {
        Message::Error("This Trace owns a Geometry. Cannot be concatenated.");
        return;
    }
    m_traces.push_back(t);
}

void Trace::insert(int i, Trace* t)
{
    std::vector<Trace*>::iterator it = m_traces.begin();
    it +=i;
    m_traces.insert(it, t);
}


Trace* Trace::getTrace(int i)
{
    if (i < getNumberOfTrace()) {
        return m_traces[i];
    } else {
        Message::Error("You are trying to access to an undefined memory block.");
        return nullptr;
    }
}

void Trace::setGeometry(Geometry *geo)
{
    if (!isBlock()) {
        m_geo = geo;
    } else {
        Message::Error("This Trace is block. Cannot own a geometry.");
        return;
    }
}

void Trace::extend(Trace *t)
{
    if (t->isBlock()) {
        for (int i=0; i<t->getNumberOfTrace(); i++)
            push_back(t->getTrace(i));
    } else {
        push_back(t);
    }
}

Trace Trace::flatize()
{
    if (isBlock()) {
        std::string name = m_name + "-flat";
        Trace tmp(name);

        Trace *T;
        for (int i=0; i<getNumberOfTrace(); i++) {
            T = getTrace(i);
            if (T->isBlock()) {
                Trace t = T->flatize();
                tmp.extend(&t);
            } else {
                tmp.extend(T);
            }
        }
        return tmp;
    } else {
        return *this;
    }
}

bool Trace::isComparable(Trace *q)
{
    if (isBlock() && q->isBlock()) {
        Trace T = this->flatize();
        Trace Q = q->flatize();
        if (T.getNumberOfTrace() != Q.getNumberOfTrace()) {
            return false;
        } else {
            for (int i=0; i<T.getNumberOfTrace(); i++) {
                if (T.getTrace(i) != Q.getTrace(i))
                    return false;
            }
        }
        return true;
    } else if (this == q) {
        return true;
    } else {
        return false;
    }
}

bool Trace::isSameStructure(Trace *t)
{
    if (isBlock() && t->isBlock()) {
        if (getNumberOfTrace() != t->getNumberOfTrace()) {
            return false;
        } else {
            for (int i=0; i<getNumberOfTrace(); i++) {
                if (!getTrace(i)->isSameStructure(t->getTrace(i)))
                    return false;
            }
            return true;
        }
    } else if (getNumberOfTrace()+t->getNumberOfTrace() == 0) {
        return true;
    } else {
        return false;
    }
}

int Trace::compare(Trace *t)
{
    if (!isComparable(t)) {
        return 0;
    } else {
        if (this == t) {
            return 1;
        } else if (isSameStructure(t)) {
            return 2;
        } else {
            return 3;
        }
    }
}

int Trace::getSize() {
    if (isBlock()) {
        Trace t = flatize();
        return t.getNumberOfTrace();
    } else {
        return 1;
    }
}

Scalar Trace::getValue(Scalar k, int i, int j, std::string type, Trace *test)
{
    if (type == "SL")
        return m_fs->SL(k, i, j, test);
    if (type == "DL")
        return m_fs->DL(k, i, j, test);
    if (type == "AL")
        return m_fs->AL(k, i, j, test);
    if (type == "HL")
        return m_fs->HL(k, i, j, test);

    if (type == "Id")
        return m_fs->Id(i, j, test);

}
