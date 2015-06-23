#pragma once

#include <Common/Type.h>
#include <Core/Matrix.h>

#include <vector>
#include <list>
#include <map>
#include <utility> //for pair


class Node
{
private:
    std::string m_op;
    Operator *m_left;
    Operator *m_right;
    double *m_scalar;
public:
Node(std::string s="", Operator *l=NULL, Operator *r=NULL, double *v=NULL):m_op(s), m_left(l), m_right(r), m_scalar(v)
    {}
    void set(std::string s, Operator *l, Operator *r){m_op=s; m_left=l; m_right=r;}
    void set(std::string s, Operator *l, double *v){m_op=s; m_left=l; m_scalar=v;}
};


class Operator
{
private:
    Shape m_shape;
    Shape m_Shape; // (0,0) by Shape constructor
    void createOperator(int row, int col);

    // change with a new type but how ?
    /// so UGLY !!!
    std::list<int> m_rows;
    std::list<int> m_cols;
    std::list<Operator*> m_ops;
//Proposition pour concilier tout ça:
    std::map<std::pair<int, int>, Operator *> m_listOfOp;
public:
    Operator(int row=0, int col=0, bool management = false);
    Operator(Shape shape, bool management = false);

    //Purely virtual for Operator
    virtual double getValue(int i, int j) = 0;
    
    //Functions to build some blocks
    void addBlock(int row, int col, Operator* newop);
    void addBlock(Shape shape, Operator* newop);

    //Every operator is associated to a node
    Node node;

    Matrix assemble();

    //Algebra on operator
//double should be changed to EBScalar
    Operator operator+(Operator other);
    Operator operator*(double scalar);

    void Print();

    Shape get_shape(){return m_shape;};
    Shape get_Shape(){return m_Shape;};


    // store the size of the banded block
    std::vector<int> banded_rows;
    std::vector<int> banded_cols;

    bool isInList(int, std::list<int>);
    bool isCheckAndUpdate_shapes(int, int, Coord);
};
