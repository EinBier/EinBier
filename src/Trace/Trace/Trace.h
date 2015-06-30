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

private:
    //"Node" part
    std::vector<Trace *> m_traces;
public:
Trace():Trace(0, "lol"){}
    explicit Trace(int n):Trace(n, "lol"){}
    explicit Trace(std::string name):Trace(0, name){}
    explicit Trace(int n, std::string name){createTrace(n, name);}

    virtual int getBlockSize(){return m_traces.size();}
    virtual void Print(bool isEnd = true);
    virtual void createTrace(int n, std::string name);
    virtual bool isElementary(){return false;}
    virtual void push_back(Trace *t);
};

class TraceElementary:public Trace
{
public:
    virtual int getBlockSize(){return 1;}
    virtual void createTrace(int n, std::string name) =0;
    virtual bool isElementary(){return true;}
    virtual void push_back(Trace *t){return;}
};

class TraceElem:public TraceElementary
{
public:
    TraceElem(std::string name){createTrace(0, name);}
     void createTrace(int n, std::string name){m_n = n; m_name=name;}
};
