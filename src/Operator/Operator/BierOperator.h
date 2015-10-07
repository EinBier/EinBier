#pragma once

#include<Common/Shape.h>
#include<Trace/Trace.h>

#include<vector>
#include<string>


class CoreOperator
{
    static std::string ELEMENTARY;
    static std::string UNARY;
    static std::string BINARY;
    static std::string BLOCK;

    struct node {
	std::string m__type; //elementary (""), unary, binary, block
	std::string m__operation;
	std::vector<int> m__ids;       //ids of Operator
	std::vector<Shape> m__indices; //indices of the Operator (Shape is used as index)
	double m__scalar; // for binary with scalar value
	//double should be changed to EinBierScalar

        // constructor
        node() {
	    reset();
	}
	
	voir reset(){
	    m__type = ELEMENTARY;
	    m__operation = "";
	    m__ids.resize(0);
	    m__indices.resize(0);
	    m__scalar= 0.;
	}

	//Binary operation between CoreOperator
        void setBinary(std::vector<int> id, std::string operation) {
	    if(id.size() == 1)
	    {
		setUnary(id[0], operation);
		return;
	    }
	    reset();
	    m__type = BINARY;
	    m__operation = operation;
	    m__ids = ids;
	    m__indices.resize(1);
	    m__indices[0].set(0,0);
        }

	//Binary operation with a scalar
        void setBinary(int id, double scalar, std::string operation) {
	    reset();
	    m__type = BINARY;
	    m__operation = operation;
	    m__scalar = scalar;
	    m__ids.resize(1);
	    m__ids = id;
	    m__indices.resize(1);
	    m__indices[0].set(0,0);
        }
	
	//Unary operation
        void setUnary(int id, std::string operation) {
	    reset();
	    m__type = UNARY;
	    m__operation = operation;
	    m__ids.resize(1);
	    m__ids[0] = id;
	    m__indices.resize(1);
	    m__indices[0].set(0,0);
        }

        void setShape(int row, int col){
	    if(row <= 0 || col <= 0)
		return;
	    reset();
	    m__type = BLOCK;
	    m__ids.resize(row*col, -1);
	    m__indices.resize(row*col);
        }

	std::string set_type(std::string type) {m__type = type;}
	std::string set_operation(std::string operation){m__operation = operation;}
	std::string get_type() const {return m__type;}
	std::string get_operation() const {return m__operation;}
	int get_id(int row, int col) const; //return the corresponding indices
	CoreOperator* getCoreOperator(int row, int col) const; //return the pointer to the corresponding indices
	int size() const {return m__ids.size();} // useless I think
    };

protected:
    int m_id; // My id
    CoreOperator::node m_node; // the operators (if it's a node)
    Shape m_shape;
    Trace *m_dof, *m_trial;
    void createCoreOperator(int row, int col);

public:
    explicit CoreOperator(int id):CoreOperator(id, 1, 1){}
    explicit CoreOperator(int id, int row, int col); // This should be changed for Traces argument
//    explicit CoreOperator(const CoreOperator &op);

//    void operator=(const CoreOperator& rhs);
    //   CoreOperator operator=(CoreOperator rhs);

//    friend CoreOperator operator+(CoreOperator &lhs, const CoreOperator& rhs);
//    bool checkSize(const CoreOperator &you) const;//Compare size between two CoreOperators
    
    CoreOperator::node get_node(){return m_node;}
/*    void setBlockSize(int nrow, int ncol);
    void setBlock(int k, int l, CoreOperator *op);
    void setTraces(Trace *dof, Trace *trial);
    virtual bool isElementary();
    void Print(bool isEnd = true);
    void PrintShape();
    bool checkSize();//For block structure: check sizes between traces
    void getElementaryBlockSize(std::vector<int> *vec) const;
    void getBlockSize(std::vector<int> *vec) const;
    int get_id() const{return m_id;}
*/
    virtual std::string WhatIsMyType() const {return m_node.get_type();};
};


class CoreOperatorElementarty: public CoreOperator
{
    virtual std::string WhatIsMyType(){return ELEMENTARY;}

};
