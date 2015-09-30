#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>
#include<iostream>


std::string Trace::ELEMENTARY = "elementary";
std::string Trace::BLOCK = "block";
std::string Trace::UNDEFINED = "undefined";

Trace::Trace(std::string name){
    std::cout << "Trace: " << name << std::endl;
    m_name = name;
    m_traces.resize(0);
    m_type = "";
    m_geo = nullptr;
    m_type = Trace::UNDEFINED;
}

Trace::Trace(std::string name, Geometry* geo) : Trace(name){
    m_geo = geo;
}


void Trace::Print(bool isRoot)
{
    if(isRoot)
        std::cout << m_name << " = ";
    if(getType() == Trace::BLOCK){
        std::cout<<"[";
        int nt = m_traces.size();
        for (int i = 0; i < nt; i ++){
            m_traces[i]->Print(false);
            if(i < nt-1)
                std::cout << ", ";
        }
        std::cout << "]";
    } else {
        if(isRoot)
            std::cout<<"[";
        if(getType() == Trace::UNDEFINED)
            std::cout << "~";
        std::cout << m_name;
        if(isRoot)
            std::cout<<"]";
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

void Trace::setGeo(Geometry *geo){
    if(getType() != Trace::BLOCK){
        m_geo = geo;
        m_type = Trace::ELEMENTARY;
    } else {
        Message::Error("This Trace is %s.", Trace::BLOCK.c_str());
    }
    return;
}


