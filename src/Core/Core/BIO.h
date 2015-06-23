#pragma once

#include <Core/Operator.h>

class BIO : public Operator
{
public:
    BIO(int r = 0, int c = 0, bool management=false);
    BIO(Shape shape, bool management = false);
    ~BIO();
//	BIO(Trace, Trace);
    
    virtual double getValue(int, int);
};
