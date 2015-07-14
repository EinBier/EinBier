#pragma once

#include<Trace/Trace.h>

#include<vector>
#include<string>

class CoreOperator
{
    struct node {
	std::string m__type; //elementary (""), unary, binary, block
	std::string m__operation;
	Shape m__shape;
	std::vector<int> m__ids;
	double m__scalar; // for binary with scalar value

        // constructor
        node() {
	    m__type = "";
	    m__ids.resize(0);
	    m__indices.resize(0);
	    m__operation = "";
	    m__scalar= 0.;
	}

        void setBinary(std::vector<int> id, std::string operation) {
	    if(id.size() == 1)
	    {
		setUnary(id[0], operation);
		return;
	    }
	    m__ids.resize(1);
	    m__ids[0] = ids;
	    m__operation = operation;
	    m__type = "binary";
        }

        void setBinary(int id, double scalar, std::string operation) {
	    m__scalar = scalar;
	    m__ids.resize(1);
	    m__ids[0].resize(1);
	    m__ids[0] = id;
	    m__operation = operation;
	    m__type = "binary";
        }

        void setUnary(int id, std::string operation) {
	    m__ids.resize(1);
	    m__ids[0] = ids;
	    m__operation = operation;
	    m__type = "unary";
        }

        void setBlockSize(int row, int col){
	    if(row <= 0 || col <= 0)
		return;
	    m__indices.resize(row);
	    m__indices[0].resize(col);
	    m__ids.resize(row*col);
	    m__ids[0] = ids;
	    m__operation = operation;
	    m__type = "unary";
        }



        node(int id, std::string operation) {
	    m__ids.resize(1);
	    m__operation = operation;
        }
	CoreOperator* get_left() const;
	CoreOperator* get_right() const;
	std::string get_operation() const {return m__operation;}
	int get_left_id() const {return m__ids[1];}
	int get_right_id() const {return m__ids.size()>1?m__ids[1]:-1;}
	int size() const {return m__ids.size();}
	void resize(int n) {m__ids.resize(n);}
    };

protected:
    int m_id;
    CoreOperator::node m_operators;              // the operators (if it's a node)
    std::vector<std::vector<int> > m_blocks; //the blocks (if it's a block operator)
    Trace *m_dof, *m_trial;

    void createCoreOperator(int row, int col);

public:
    explicit CoreOperator():CoreOperator(0, 0, false){}
    explicit CoreOperator(bool management):CoreOperator(0, 0, management){}
    explicit CoreOperator(int row, int col):CoreOperator(row, col, false){}
    explicit CoreOperator(int row, int col, bool management);
//    explicit CoreOperator(const CoreOperator &op);

    void operator=(const CoreOperator& rhs);
    //   CoreOperator operator=(CoreOperator rhs);

    friend CoreOperator operator+(CoreOperator &lhs, const CoreOperator& rhs);
    bool checkSize(const CoreOperator &you) const;//Compare size between two CoreOperators
    
    CoreOperator::node get_node(){return m_operators;}
    void setBlockSize(int nrow, int ncol);
    void setBlock(int k, int l, CoreOperator *op);
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


class CoreOperatorElementarty: public CoreOperator
{
    virtual std::string WhatIsMyType(){return "elementary";}

};
