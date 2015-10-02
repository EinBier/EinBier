#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>
#include<iostream>


std::string Trace::ELEMENTARY = "elementary";
std::string Trace::BLOCK = "block";
std::string Trace::UNDEFINED = "undefined";

Trace::Trace(std::string name){
    m_name = name;
    m_geo = nullptr;
    m_method = -1;
    m_traces.resize(0);
    m_type = Trace::UNDEFINED;
    std::cout << "Trace " << name << " is initialized." << std::endl;
}

Trace::Trace(std::string name, Geometry* geo) : Trace(name){
    setGeometry(geo);
}

Trace::Trace(std::string name, Geometry* geo, int method) : Trace(name, geo){
    m_method = method;
}


void Trace::Print(bool isRoot)
{
    if(isRoot){
        std::cout << "Trace: " << m_name << std::endl;
        std::cout << "\tGeometry: " << m_geo << std::endl;
        std::cout << "\tMethod: " << m_method << std::endl;
        std::cout << "\tType: " << m_type << std::endl;
        std::cout << m_name << " = ";
    }
    if(getType() == Trace::BLOCK){
        std::cout<<"[";
        int nt = getNumberOfTrace();
        for (int i = 0; i < nt; i ++){
            m_traces[i]->Print(false);
            if(i < nt-1)
                std::cout << ", ";
        }
        std::cout << "]";
    } else {
        if(getType() == Trace::UNDEFINED)
            std::cout << "~";
        std::cout << m_name ;
        if(isRoot)
            std::cout << " -- obvious, right ? :-)";
    }
    if(isRoot)
        std::cout << std::endl;
}


void Trace::push_back(Trace* t){
    if(getType() == Trace::ELEMENTARY)
    {
        Message::Error("This Trace is %s.", Trace::ELEMENTARY.c_str());
        return;
    }
    m_traces.push_back(t);
    m_type = Trace::BLOCK;
}

void Trace::setGeometry(Geometry *geo){
    if(getType() != Trace::BLOCK){
        m_geo = geo;
        m_type = Trace::ELEMENTARY;
    } else {
        Message::Error("This Trace is %s.", Trace::BLOCK.c_str());
    }
    return;
}

void Trace::extend(Trace *t){
    if(t->getType() == Trace::BLOCK){
        for(int i=0; i<t->getNumberOfTrace(); i++)
            push_back(t->m_traces[i]);
    } else {
        push_back(t);
    }
}

Trace* Trace::flatize(){
    Trace *tmp;

    if(getType() == Trace::BLOCK){
        std::string name = m_name + "-flat";
        tmp = new Trace(name);

        Trace *T, *t;
        for(int i=0; i<getNumberOfTrace(); i++){
            T = m_traces[i];
            if(T->getType() == Trace::BLOCK){
                for(int j=0; j<T->getNumberOfTrace(); j++){
                    tmp->extend(T->m_traces[j]->flatize());
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

bool Trace::isComparable(Trace *q){
    if(getType() == Trace::BLOCK
       && q->getType() == Trace::BLOCK){
        Trace *T = this->flatize();
        Trace *Q = q->flatize();
        if(T->getNumberOfTrace() != Q->getNumberOfTrace()){
            return false;
        } else {
            for(int i=0;i<T->getNumberOfTrace();i++){
                if(T->m_traces[i] != Q->m_traces[i])
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

bool Trace::isSameStructure(Trace *t){
    if(getType() == Trace::BLOCK
       && t->getType() == Trace::BLOCK){
        if(getNumberOfTrace() != t->getNumberOfTrace()){
            return false;
        } else {
            for(int i=0; i<getNumberOfTrace(); i++){
                if(!m_traces[i]->isSameStructure(t->m_traces[i]))
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

int Trace::compare(Trace *t){
    if(!isComparable(t)){
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
