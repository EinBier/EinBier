#pragma once

#include <Geometry/Geometry.h>
#include <Method/Method_ENUM.h>
#include <vector>
#include <string>

class Trace
{
protected:
    std::string m_name;
    Geometry *m_geo;
    int m_method;               // method of resolution ?

    std::string m_type;         // "elementary" | "block" | "undefined"

    static std::string ELEMENTARY;
    static std::string BLOCK;
    static std::string UNDEFINED;

//private:
public:
    std::vector<Trace*> m_traces;

public:
    Trace(std::string name = "default");
    Trace(std::string name, Geometry* geo);
    Trace(std::string name, Geometry* geo, int method);

    int getNumberOfTrace(){return m_traces.size();}
    void Print(bool isRoot = true);

    void push_back(Trace* t);

    void setGeometry(Geometry *geo);
    void setMethod(int m){m_method = m;}

    std::string getType(){return m_type;}

    void extend(Trace*);
    Trace* flatize();
    bool isComparable(Trace*);
    bool isSameStructure(Trace*);
    int compare(Trace*);
};

