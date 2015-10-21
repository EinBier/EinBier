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


private:
    std::vector<Trace*> m_traces;

    bool isDefined();
    bool isComparable(Trace*);
    bool isSameStructure(Trace*);


public:
    Trace(std::string name = "default");
    Trace(std::string name, Geometry* geo);
    Trace(std::string name, Geometry* geo, int method);

    ~Trace();

    int getNumberOfTrace(){return m_traces.size();}
    void Print(bool isRoot = true);

    bool isBlock();

    void push_back(Trace* t);
    void insert(int, Trace*);
    Trace* getTrace(int);
    void setTrace(int);

    void setGeometry(Geometry *geo);
    void setMethod(int m){m_method = m;}

    std::string getType();

    void extend(Trace*);
    Trace flatize();
    int compare(Trace*);

    int getSize();

    bool operator==(Trace *t){return isComparable(t);}
    bool operator!=(Trace *t){return !isComparable(t);}
    void operator+=(Trace *t){push_back(t);}
    Trace* operator[](int i){return getTrace(i);}
};
