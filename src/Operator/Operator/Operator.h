#pragma once

#include<Trace/Trace.h>

#include<vector>
#include<string>

class Operator
{
    struct node {
	std::vector<int> m__ids;
	std::string m__operation;

        // constructor
        node() {
	    m__ids.resize(0);
	    m__operation = "";
	}

        node(std::vector<int> ids, std::string operation) {
	    m__ids = ids;
	    m__operation = operation;
        }
        node(int id, std::string operation) {
	    m__ids.resize(1);
	    m__operation = operation;
        }
	Operator* get_left() const;
	Operator* get_right() const;
	std::string get_operation() const {return m__operation;}
	int get_left_id() const {return m__ids[1];}
	int get_right_id() const {return m__ids.size()>1?m__ids[1]:-1;}
	int size() const {return m__ids.size();}
	void resize(int n) {m__ids.resize(n);}
    };

protected:
    int m_id;
    Operator::node m_operators;              // the operators (if it's a node)
    std::vector<std::vector<int> > m_blocks; //the blocks (if it's a block operator)
    Trace *m_dof, *m_trial;

    void createOperator(int row, int col, bool management);

public:
    explicit Operator():Operator(0, 0, false){}
    explicit Operator(bool management):Operator(0, 0, management){}
    explicit Operator(int row, int col):Operator(row, col, false){}
    explicit Operator(int row, int col, bool management);
//    explicit Operator(const Operator &op);
    ~Operator();

    Operator operator=(const Operator& rhs);
    //   Operator operator=(Operator rhs);

    friend Operator operator+(Operator &lhs, const Operator& rhs);
    bool checkSize(const Operator &you) const;//Compare size between two Operators
    
    Operator::node get_node(){return m_operators;}
    void setBlockSize(int nrow, int ncol);
    void setBlock(int k, int l, Operator *op);
    void setTraces(Trace *dof, Trace *trial);
    virtual bool isElementary();
    void Print(bool isEnd = true);
    void PrintShape();
    bool checkSize();//For block structure: check sizes between traces
    void getElementaryBlockSize(std::vector<int> *vec) const;
    void getBlockSize(std::vector<int> *vec) const;
    virtual std::string WhatIsMyType() const;
    int get_id() const{return m_id;}
};


class OperatorElementarty: public Operator
{
    virtual std::string WhatIsMyType(){return "elementary";}

};
