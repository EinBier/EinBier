#pragma once

#include <Common/Type.h>
#include <Core/Matrix.h>

#include <string>
#include <vector>
#include <list>
//#include <map>
//#include <utility> //for std::pair


/* NOTE: */
/* double need to be converted into EBScalar */

class Operator;

class Node
{
public:
    std::string m_op;
    Operator *m_left;
    Operator *m_right;
    double *m_scalar;

public:

    Node(std::string s="", Operator *l=NULL, Operator *r=NULL, double *v=NULL)
        : m_op(s), m_left(l), m_right(r), m_scalar(v) {};

    void set(std::string s, Operator *l, Operator *r)
    {m_op=s; m_left=l; m_right=r;};
    void set(std::string s, Operator *l, double *v)
    {m_op=s; m_left=l; m_scalar=v;};
};


class Operator
{
private:
    Node m_node;         // store Arithmetic operations

protected:

    int m_id;            // used by OperatorHandler
    Shape m_shape;       // shape matrix
    Shape m_shape_block; // (0,0) by Shape constructor

    void createOperator(int row, int col, bool management);

    // store the size of the banded blocks
    std::vector<int> m_banded_rows;
    std::vector<int> m_banded_cols;

    bool isInList(int, std::list<int>);
    bool isCheckAndUpdate_shapes(int, int, Shape); // to slip in parts

    // change with a new type but how ?
    /// so UGLY !!!
    std::list<int> m_rows;
    std::list<int> m_cols;
    std::list<Operator*> m_ops;
	// alternative with map
    // std::map<std::pair<int, int>, Operator *> m_listOfOp;


public:

    Operator(int row=0, int col=0, bool management = true);
    Operator(Shape shape, bool management = false);
    ~Operator();

    void setBlock(int row, int col, Operator* op);
    void setBlock(Shape shape, Operator* op);

    Operator operator+(Operator & other);
    Operator operator*(double scalar);

    virtual Matrix assemble();
    virtual double getValue(int i, int j){};

    void Print();

    int get_id(){return m_id;};
    Shape get_shape(){return m_shape;};
    Shape get_shape_block(){return m_shape_block;};
    Node* get_node_ptr(){return &m_node;};

    void set_shape(Shape s)
    {m_shape.set_row(s.get_row());
        m_shape.set_col(s.get_col()); };
    void set_shape_block(Shape s)
    {m_shape_block.set_row(s.get_row());
        m_shape_block.set_col(s.get_col()); };
};
