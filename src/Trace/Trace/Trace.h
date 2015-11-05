#pragma once

#include <Geometry/Geometry.h>
#include <FunctionSpace/FunctionSpace.h>
#include <vector>
#include <string>

class Trace
{
protected:
    std::string m_name;
    Geometry *m_geo;
    FunctionSpace* m_fs;

    int m_method;


private:
    std::vector<Trace*> m_traces;

    bool isComparable(Trace*);
    bool isSameStructure(Trace*);


public:
    Trace(std::string name = "default");
    Trace(std::string name, Geometry* geo);
    Trace(std::string name, Geometry* geo, int method);

    ~Trace();

    int getNumberOfTrace(){return m_traces.size();}
    void Print(bool isRoot = true);

    bool isDefined();
    bool isBlock();

    void push_back(Trace* t);
    void insert(int, Trace*);
    Trace* getTrace(int);
    void setTrace(int);

    void setGeometry(Geometry *geo);
    void setFunctionSpace(FunctionSpace *fs){m_fs = fs;}

    std::string getType();

    void extend(Trace*);
    Trace flatize();
    int compare(Trace*);

    int getSize();

    bool operator==(Trace *t){return isComparable(t);}
    bool operator!=(Trace *t){return !isComparable(t);}
    void operator+=(Trace *t){push_back(t);}
    Trace* operator[](int i){return getTrace(i);}

    Scalar getValue(Scalar k, int i, int j, std::string type, Trace *test);
};
