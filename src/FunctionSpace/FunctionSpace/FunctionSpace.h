#pragma once

#include<Common/Type.h>
#include<Geometry/Geometry.h>

#include<vector>
#include<string>


class Trace;

class FunctionSpace {

public:

    Geometry *geo;

    virtual Scalar SL(Scalar k, int i, int j, Trace *test) {return 0.;}
    virtual Scalar DL(Scalar k, int i, int j, Trace *test) {return 0.;}
    virtual Scalar AL(Scalar k, int i, int j, Trace *test) {return 0.;}
    virtual Scalar HL(Scalar k, int i, int j, Trace *test) {return 0.;}

    virtual Scalar Id(int i, int j, Trace *test) {return 0.;}

    virtual int getDof() {return 0;}
};


class Dummy : public FunctionSpace {

public:
    int m_mode;

    Scalar Id(int i, int j, Trace *test);
};
