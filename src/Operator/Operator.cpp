#include<Common/Message.h>

#include <Trace/Trace.h>

#include <Operator/Operator.h>
#include <Operator/Barman.h>

#include <iostream>


Operator::~Operator()
{
    Barman::DecreaseNumberOfPointer(m_id);
}

Operator::Operator()
{
    m_id = -1;
}

Operator::Operator(int row, int col)
{
    createOperator(row, col);
}
/*
Operator::Operator(const Operator &op)
{
    m_id = Barman::addOperator(this, false);
    m_operators = op.m_operators;
    m_blocks = op.m_blocks;
    m_dof = op.m_dof;
    m_trial = op.m_trial;    
}
*/

void Operator::createOperator(int row, int col)
{
    m_id = Barman::createCoreOperator(row, col);
    return;
}


void Operator::setBlockSize(int nrow, int ncol){
    m_blocks.resize(nrow);
    for (int i = 0; i < nrow ; i ++)
	m_blocks[i].resize(ncol, -1);
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


void Operator::getBlockSize(std::vector<int> *vec) const
{
    vec->resize(2, 0);
    (*vec)[0] = m_blocks.size();
    if(m_blocks.size() > 0)
	(*vec)[1] = m_blocks[1].size();
    return;
}


void Operator::getElementaryBlockSize(std::vector<int> *vec) const
{
    vec->resize(2);
    std::string type = WhatIsMyType();
    if(type == "elementary")
    {
	(*vec)[0] = 1;
	(*vec)[1] = 1;
    }
    else if(type == "node")
    {
	m_operators.get_left()->getElementaryBlockSize(vec);
    }
    else if(type == "block")
    {
	int nrows = m_blocks.size();
	int ncols = m_blocks[0].size();
	for (int i = 0; i < nrows; i++)
	{
	    std::vector<int> row_tmp;
	    Operator *op;
	    //Find the first term non null in the column
	    for (int j = 0; j < ncols; j++)
	    {
		op = Barman::get_Operator_ptr(m_blocks[i][j]);
		if(op != nullptr)
		    break;
		else if (j == ncols-1)
		{
		    Message::Error("Problem a zero line/column exists! Cannot compute the size...");
		    return;
		}
	    }
	    op->getElementaryBlockSize(&row_tmp);
	    (*vec)[0] += row_tmp[0];
	}
	for (int j = 0; j < ncols; j++)
	{
	    std::vector<int> col_tmp;
	    Operator *op;
	    //Find the first term non null in the line
	    for (int i = 0; i < nrows; i++)
	    {
		op = Barman::get_Operator_ptr(m_blocks[i][j]);
		if(op != nullptr)
		    break;
		else if (i == nrows-1)
		{
		    Message::Error("Problem a zero line/column exists! Cannot compute the size...");
		    return;
		}
	    }
	    op->getElementaryBlockSize(&col_tmp);
	    (*vec)[1] += col_tmp[1];
	}
    }
    return;
}


bool Operator::checkSize(const Operator &you) const
{
    std::string myType = WhatIsMyType();
    std::string yourType = you.WhatIsMyType();
    if(myType == "elementary" && yourType == "elementary")
	return true;
    if(myType == "node")
    {
	Operator *op_left = m_operators.get_left();
	return op_left->checkSize(you);
    }
    if(yourType=="node")
    {
	Operator *op_left = you.m_operators.get_left();
	return op_left->checkSize(*this);
    }
//Ok, there is only blocks here
    std::vector<int> my_shape(2,0), your_shape(2,0);
    getBlockSize(&my_shape);
    you.getBlockSize(&your_shape);
    if(my_shape != your_shape)
	return false;
    for (int i = 0; i < my_shape[0]; i ++)
    {
	for (int j = 0; j < my_shape[1]; j++)
	{
	    Operator *mine_aux = Barman::get_Operator_ptr(m_blocks[i][j]);
	    Operator *yours_aux = Barman::get_Operator_ptr(you.m_blocks[i][j]);
	    if(mine_aux == nullptr  || yours_aux == nullptr)
		continue;
	    bool res_aux = mine_aux->checkSize(*yours_aux);
	    if (res_aux == false)
		return false;
	}
    }
    return true;
}

std::string Operator::WhatIsMyType() const
{
    std::string type = "elementary";
    if(m_operators.size() > 0)
	type = "node";
    if(m_blocks.size() > 0)
	type = "block";
    return type;
}

void Operator::Print(bool isEnd){
    std::string type = WhatIsMyType();
    if(isEnd)
	Message::Info("I'm the Operator %d, I'm a %s and here is my structure:", m_id, type.c_str());
    else
	Message::Debug("Op id = %d",m_id);

    if(type == "elementary")
	std::cout << m_id;

    if(type == "node")
    {
	if(m_operators.size() > 1) // Binary operator
	{
	    std::cout << "(";
	    m_operators.get_left()->Print(false);
	    std::cout << " " << m_operators.get_operation()<< " ";
	    m_operators.get_right()->Print(false);
	    std::cout << ")";
	}
	else // Unary operator
	{
	    std::cout << m_operators.get_operation();
	    std::cout << "(";
	    m_operators.get_left()->Print(false);
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
		if(op == nullptr)
		    std::cout << "Z";
		else
		    op->Print(false);
		std::cout << " ";
	    }
	    std::cout << "]";
	    if(i < m_blocks.size()-1)
		std::cout<<std::endl;
	}
    }

    if(isEnd)
	std::cout << std::endl;
    return;
}

void Operator::PrintShape()
{
    std::vector<int> shape(2);
    getElementaryBlockSize(&shape);
    Message::Info("Operator %d shape : (%d, %d) (in Elementary blocks)", m_id, shape[0], shape[1]);
}

bool compareSize(Operator *op)
{


}


Operator operator+(Operator &lhs, const Operator& rhs)
{
    Barman::Print();
    Operator *res = new Operator(true);
    //check size!
    bool checkS = lhs.checkSize(rhs);
    if(!checkS || lhs.m_dof != rhs.m_dof || lhs.m_trial != rhs.m_trial)
    {
	Message::Error("Operator are not of the same block structure! result is crap...");
	res->setBlockSize(-1, -1);
	return *res;
    }
    //Set sizes;
    res->m_dof = lhs.m_dof;
    res->m_trial = lhs.m_trial;
    res->m_operators.m__operation = "+";
    res->m_operators.m__ids.resize(2);
    res->m_operators.m__ids[0] = lhs.m_id;
    res->m_operators.m__ids[0] = rhs.m_id;    
    res->Print();
    return *res;
}

/*Operator Operator::operator=(Operator rhs)
{
    Message::Debug("entering copy");
    if (this != &rhs)
    {
	//copy everything except id.
	m_operators = rhs.m_operators;
	m_blocks = rhs.m_blocks;
	m_dof = rhs.m_dof;
	m_trial = rhs.m_trial;
    }
    return *this;
}
*/

void Operator::operator=(const Operator& rhs)
{
    Barman::Print();
    Message::Debug("entering const copy");
    if (this != &rhs)
    {
	//copy everything except id.
	m_operators = rhs.m_operators;
	m_blocks = rhs.m_blocks;
	m_dof = rhs.m_dof;
	m_trial = rhs.m_trial;
    }
    Barman::Print();
//    return *this;
}
