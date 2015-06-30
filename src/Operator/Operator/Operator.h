#pragma once

#include<Trace/Trace.h>

#include<vector>

class Operator
{
    struct node {
	std::vector<int> m__ids;
	std::string m__operation;

        // constructor
        element() {
	    m__ids.resize(0);
	    m__operation = "";
	}

        element(std::vector<int> ids, std::string operation) {
	    m__operators = ids;
	    m__operation = operation;
        }
        element(int id, std::string operation) {
	    m__operators.resize(1);
	    m__operators[0] = stdids;
	    m__operation = operation;
        }
    };

protected:
    int m_id;
    std::vector<Operator::node> m_operators; // the operators (if it's a node)
    std::vector<std::vector<int ids> > m_blocks; //the blocks (if exists)
    Trace *m_dof, *m_trial;

public:

    int get_id(){return m_id;}
    void setBlockSize(int nrow, int ncol);
    void setBlock(int k, int l, Operator *op);
    void setTraces(Trace *dof, Trace *trial);
    virtual bool isElementary(){return ((m_blocks.size() + m_operators.size())==0);}
};

