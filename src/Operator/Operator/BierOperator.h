#pragma once

#include<Common/Shape.h>
#include<Barman/Bier.h>
#include<Trace/Trace.h>

#include<vector>
#include<string>

class BNode {

private:

    // "", "unary", "binary", "block"
	std::string m_type;

    // "", "+" "-" "*"
	std::string m_operation;
    int m_OpIdR;
    int m_OpIdL;

	//double should be changed to EinBierScalar
    double m_scalar;

    // Block
    std::vector<int> m_OpIds;
	std::vector<Shape> m_indices;


public:
    BNode();

	void reset();

    std::string getType() {return m_type;}

    // Binary: A+B or A-B
    void set(int OpIdL, std::string operation, int OpIdR);

    // Scalar: 2*A or A*2
    void set(double scalar, int OpIdR);

    // Unary: -A or +A
    void set(std::string operation, int OpId);

    // Block
    void set(int row, int col, int OpId);
    int getBlock(int i, int j);
};

///////////////////////////////////////////
// ************************************* //
///////////////////////////////////////////

class BierOperator : public Bier {

protected:
    std::string m_name;

private:
    BNode m_node;
    Trace* m_test;
    Trace* m_trial;

    Shape m_Shape;

    // offset starts at 0
    // and finishes at Number Of Elementary Traces
    std::vector<int> m_offset_test;
    std::vector<int> m_offset_trial;

    bool m_del_test;
    bool m_del_trial;

public:
    static std::string ELEMENTARY;
    static std::string BLOCK;
    static std::string BELEM;
    static std::string UNDEFINED;

    static std::string UNARY;
    static std::string BINARY;

    BierOperator(std::string name = "default", bool in_charge = false);
    ~BierOperator();

    void Print();

    Trace getSubTrace(Trace*, int, std::vector<int>);
    Trace* getTraceTest() {return m_test;}
    Trace* getTraceTrial() {return m_trial;}

    void setTrace(Trace*, Trace*);
    void setBlock(int, int, BierOperator *);
    // getBlock(int, int)

    void setShape(Shape);
    Shape getShape();
    Shape getSize();

    BNode* getNode() {return &m_node;}
};
