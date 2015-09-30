#pragma once

#include <Geometry/Geometry.h>
#include <Method/Method_ENUM.h>
#include <vector>
#include <string>

class Trace
{
protected:
    Geometry *m_geo;
    int m_method;               // method of resolution ?
    std::string m_name;
    std::string m_type;         // "elementary" | "block" | "undefined"

    static std::string ELEMENTARY;
    static std::string BLOCK;
    static std::string UNDEFINED;

private:
    //"Node" part
    std::vector<Trace*> m_traces;

public:
    Trace(std::string name = "default");
    Trace(std::string name, Geometry* geo);

    int getNumberOfTrace(){return m_traces.size();}
    void Print(bool isRoot = true);
    std::string getType(){return m_type;}
    void push_back(Trace* t);
    void setGeo(Geometry *geo);
    void setMethod(int m){m_method = m;}
};

