#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>
#include<iostream>


Trace::Trace(std::string name)
{
    m_name = name;
    m_geo = nullptr;
    m_method = -1;
    m_traces.resize(0);
    std::cout << "Trace " << name << " is initialized." << std::endl;
}

Trace::Trace(std::string name, Geometry* geo) : Trace(name)
{
    setGeometry(geo);
}

Trace::Trace(std::string name, Geometry* geo, int method) : Trace(name, geo)
{
    m_method = method;
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
        std::cout << "Trace: " << m_name << std::endl;
        std::cout << "\tGeometry: " << m_geo << std::endl;
        std::cout << "\tMethod: " << m_method << std::endl;
        std::cout << "\tType: " << getType() << std::endl;
        std::cout << m_name << " = ";
    }
    if (isBlock()) {
        std::cout<<"[";
        int nt = getNumberOfTrace();
        for (int i = 0; i < nt; i ++) {
            m_traces[i]->Print(false);
            if(i < nt-1)
                std::cout << ", ";
        }
        std::cout << "]";
    } else {
        if (!isDefined())
            std::cout << "~";
        std::cout << m_name ;
        if (isRoot)
            std::cout << " -- obvious, right ? :-)";
    }
    if (isRoot)
        std::cout << std::endl;
}


void Trace::push_back(Trace* t)
{
    if (isDefined()) {
        Message::Error("This Trace owns a Geometry. Cannot be concatenated.");
        return;
    }
    m_traces.push_back(t);
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

Trace* Trace::flatize()
{
    Trace *tmp;

    if (isBlock()) {
        std::string name = m_name + "-flat";
        tmp = new Trace(name);

        Trace *T, *t;
        for (int i=0; i<getNumberOfTrace(); i++) {
            T = m_traces[i];
            if (T->isBlock()) {
                for (int j=0; j<T->getNumberOfTrace(); j++) {
                    tmp->extend(T->getTrace(j)->flatize());
                }
            } else {
                tmp->push_back(m_traces[i]);
            }
        }
    } else {
        tmp = this;
    }

    return tmp;
}

bool Trace::isComparable(Trace *q)
{
    if (isBlock() && q->isBlock()) {
        Trace *T = this->flatize();
        Trace *Q = q->flatize();
        if (T->getNumberOfTrace() != Q->getNumberOfTrace()) {
            return false;
        } else {
            for (int i=0;i<T->getNumberOfTrace();i++) {
                if (T->getTrace(i) != Q->getTrace(i))
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
                if (!m_traces[i]->isSameStructure(t->getTrace(i)))
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
