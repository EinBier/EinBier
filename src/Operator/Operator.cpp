#include<Common/Message.h>

#include <Trace/Trace.h>

#include <Operator/Operator.h>
#include <Operator/Barman.h>

#include <iostream>


Operator* Operator::node::get_left(){
    if(m__ids.size() < 1)
    {
	Message::Error("There is only %d Operator in the node", m__ids.size());
	return nullptr;
    }
    return Barman::get_Operator_ptr(m__ids[0]);
}

Operator* Operator::node::get_right(){
    if(m__ids.size() < 2)
    {
	Message::Error("There is only %d Operator in the node", m__ids.size());
	return nullptr;
    }
    return Barman::get_Operator_ptr(m__ids[1]);
}


Operator::~Operator()
{
    Barman::removeOperator(m_id);
}


Operator::Operator(int row, int col, bool management)
{
    createOperator(row, col, management);
}


void Operator::createOperator(int row, int col, bool management)
{
    m_operators.resize(0);
    m_blocks.resize(row);
    for (int i = 0; i < row ; i ++)
	m_blocks[i].resize(col);
    m_id = Barman::addOperator(this, management);
    return;
}


void Operator::setBlockSize(int nrow, int ncol){
    m_blocks.resize(nrow);
    for (int i = 0; i < nrow; i++)
	m_blocks[i].resize(ncol);
}


void Operator::setBlock(int k, int l, Operator *op){
    int nrow = 0, ncol = 0;
    nrow = m_blocks.size();
    if (nrow <=0)
    {
	Message::Error("Set size of operator first !");
	return;
    }
    ncol = m_blocks[0].size();
    if(ncol <=0)
    {
	Message::Error("Set size of operator first !");
	return;
    }
    m_blocks[k][l] = op->get_id();
}



void Operator::setTraces(Trace *dof, Trace *trial){
    //check the block-size (if already set) or set the block size
    int nrow = 0, ncol = 0;
    nrow = m_blocks.size();
    if (nrow >0)
    {
	ncol = m_blocks[0].size();
	if(nrow != dof->getBlockSize() || ncol !=trial->getBlockSize())
	{

	}
    }
    else
	setBlockSize(dof->getBlockSize(), trial->getBlockSize());
    m_dof = dof;
    m_trial = trial;

}


bool Operator::isElementary(){
    int nrow = m_blocks.size();
    if( nrow >0)
	return false;
    int ncol = m_blocks[0].size();
    if( ncol >0)
	return false;
    if( m_operators.size() > 0)
	return false;
    return true;
}


void Operator::Print(bool isEnd){
    std::string type = "elementary";
    if(m_operators.size() > 0)
	type = "node";
    if(m_blocks.size() > 0)
	type = "block";
    if(isEnd)
	Message::Info("I'm the Operator %d, I'm a %s and here is my structure:", m_id, type.c_str());

    if(type == "elementary")
    {
	std::cout << m_id;
    }

    if(type == "node")
    {
	if(m_operators.size() > 1)
	{
	    std::cout << "(";
	    m_operators.get_left()->Print();
	    std::cout << " " << m_operators.get_operation()<< " ";
	    m_operators.get_right()->Print();
	    std::cout << ")";
	}
	else
	{
	    std::cout << m_operators.get_operation();
	    std::cout << "(";
	    m_operators.get_left()->Print();
	    std::cout << ")";
	}
    }

    if(type == "block")
    {
	for (int i = 0; i < m_blocks.size() ; i++)
	{
	    std::cout << "[ ";
	    for (int j = 0; j < m_blocks[i].size() ; j++)
	    {
		int id = m_blocks[i][j];
		Operator *op = Barman::get_Operator_ptr(id);
		op->Print(false);
		std::cout << " ";
	    }
	    std::cout << "]"<<std::endl;
	}
    }

    if(isEnd)
	std::cout << std::endl;
    return;
}
