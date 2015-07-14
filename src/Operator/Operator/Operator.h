#pragma once

#include<vector>
#include<string>

class Operator
{
private:
    int m_id; // The id of the pointed CoreOperator
public:
//Should be :
//    explicit Operator(Trace* dof, Trace* test);
    explicit Operator();
    explicit Operator(int row, int col);
//    explicit Operator(const Operator &op);
    ~Operator();

};
