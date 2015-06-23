#pragma once

#include <Common/Type.h>
#include <Core/Matrix.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility> //for std::pair

class Operator;

class Node
{
public: //Actually, we can let everything public, right ?
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
    //Every operator is associated to a node
    Node m_node;
protected: //derived class also share these attributs
    int m_id; //id in the OperatorHandler
    Shape m_shape; // shape matrix
    Shape m_shape_block; // (0,0) by Shape constructor
    void createOperator(int row, int col, bool management);
    // store the size of the banded block
    std::vector<int> m_banded_rows;
    std::vector<int> m_banded_cols;
    bool isInList(int, std::list<int>);
    bool isCheckAndUpdate_shapes(int, int, Shape); //Maybe to slip in parts

    // change with a new type but how ?
    /// so UGLY !!!
    std::list<int> m_rows;
    std::list<int> m_cols;
    std::list<Operator*> m_ops;
//Proposition pour concilier tout ça ?:
//    std::map<std::pair<int, int>, Operator *> m_listOfOp;
public:
    Operator(int row=0, int col=0, bool management = false);
    Operator(Shape shape, bool management = false);
    ~Operator();

    //Methods used to build some blocks
    void setBlock(int row, int col, Operator* newop);
    void setBlock(Shape shape, Operator* newop);

    //Algebra on operator
//double should be changed to EBScalar
    Operator operator+(Operator & other);
//    Operator operator+(Operator other);
    Operator operator*(double scalar);
    
// assemble the system
    virtual Matrix assemble();
    //double should be changed to EBScalar (or... ?)
    virtual double getValue(int i, int j){}

    void Print();
//Get parameters methods:
    int get_id(){return m_id;}
    Shape get_shape(){return m_shape;}
    Shape get_shape_block(){return m_shape_block;}
    Node* get_node_ptr(){return &m_node;}
};
