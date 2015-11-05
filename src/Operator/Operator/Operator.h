#pragma once

#include<vector>
#include<string>

#include <Common/Type.h>
#include <Trace/Trace.h>
#include <Operator/BierOperator.h>


class Operator {

private:
    int m_id;

public:
    Operator(std::string name = "default");
    ~Operator();

    BierOperator* getBierOp();
    void Print();

    void setTrace(Trace*, Trace*);
    void setBlock(int, int, Operator&);

    Operator operator+();
    Operator operator-();
    Operator operator*(double scalar);

    void assemble();

    void setOperator(std::string, Scalar);
};

Operator operator*(double, Operator);
Operator operator+(Operator, Operator);
Operator operator-(Operator, Operator);
