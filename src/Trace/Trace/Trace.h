#pragma once

#include <Geometry/Geometry.h>
#include <Method/Method_ENUM.h>
#include <vector>
#include <string>

class Trace
{
protected:
    Geometry *m_geo;
    int m_method;               //Which method of resolution
    std::vector<int> m_indices; //indices of the unknown
    int m_n;                    // number of unknown
    std::string m_name;         // name
public:
Trace():Trace(0, "lol"){}
    explicit Trace(int n):Trace(n, "lol"){}
    explicit Trace(std::string name):Trace(0, name){}
    explicit Trace(int n, std::string name){createTrace(n, name);}
    virtual void Print();
    std::string TreePrint();
    virtual void createTrace(int n, std::string name){m_n = n; m_name = name;}
};


class TraceFourier:public Trace
{
public:
TraceFourier():Trace(0, "F"){}
    virtual void createTrace(int n, std::string name){m_n = n; m_name="F";}

};


class TraceA:public Trace
{
public:
TraceA():Trace(0, "A"){}
    virtual void createTrace(int n, std::string name){m_n = n; m_name="A";}
};

class TraceB:public Trace
{
public:
TraceB():Trace(0, "B"){}
    virtual void createTrace(int n, std::string name){m_n = n; m_name="B";}
};
