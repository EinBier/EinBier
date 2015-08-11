#include<Common/Message.h>
#include <Common/Shape.h>

#include <Trace/Trace.h>

#include <Operator/CoreOperator.h>
#include <Operator/Barman.h>

#include <iostream>


std::string CoreOperator::ELEMENTARY = "elementary";
std::string CoreOperator::UNARY = "unary";
std::string CoreOperator::BINARY = "binary";
std::string CoreOperator::BLOCK = "block";


CoreOperator::CoreOperator(int id, int row, int col)
{
    m_id = id;
    createCoreOperator(row, col);
}

/*
CoreOperator::CoreOperator(const CoreOperator &op)
{
    m_id = Barman::addCoreOperator(this, false);
    m_operators = op.m_operators;
    m_blocks = op.m_blocks;
    m_dof = op.m_dof;
    m_trial = op.m_trial;    
}
*/

//Is it a good idea ? It should be created from traces ... ?
void CoreOperator::createCoreOperator(int row, int col)
{
    m_dof = nullptr;
    m_trial = nullptr;
    m_shape.set(1,1);
    setBlockSize(row, col);
    m_id = Barman::addCoreOperator(this);
    return;
}


void CoreOperator::setBlockSize(int nrow, int ncol){
    if( nrow > 1 || ncol > 1)
    {
	//So, this CoreOperator is a block
	m_node.set_type("block");
	m_node.set_operation("block");
    }
    if( nrow < 1 || ncol < 1)
	return; //That's a joke ?
    m_shape.set(nrow, ncol);
}


void CoreOperator::setBlock(int k, int l, CoreOperator *op){

    //Check if we have the right;
    if(whatIsMyType == "")
    {
	setBlockSize(k,l);//Set a new size and a Block structure.
	m_node.set_type("block");
	m_node.set_operation("block");
    }
    if(whatIsMyType != "block")
    {
	Message::Warning("Operator %d was not a block before ! Now it is...", m_id);
	m_node.set_type("block");
	m_node.set_type("operator");
    }
    int nrow = 0, ncol = 0;
    nrow = m_shape.get_row();
    ncol = m_shape.get_col();
    if(nrow < k)
    {
	Message::Error("Changing size of Operator %d", m_id);
	m_shape.set_row(k);
    }
    if(ncol < l)
    {
	Message::Error("Changing size of Operator %d", m_id);
	m_shape.set_col(l);
    }
    
    m_node.setBlock

    m_blocks[k][l] = op->get_id();
}



void CoreOperator::setTraces(Trace *dof, Trace *trial){
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


bool CoreOperator::isElementary(){
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


void CoreOperator::getBlockSize(std::vector<int> *vec) const
{
    vec->resize(2, 0);
    (*vec)[0] = m_blocks.size();
    if(m_blocks.size() > 0)
	(*vec)[1] = m_blocks[1].size();
    return;
}


void CoreOperator::getElementaryBlockSize(std::vector<int> *vec) const
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
	    CoreOperator *op;
	    //Find the first term non null in the column
	    for (int j = 0; j < ncols; j++)
	    {
		op = Barman::get_CoreOperator_ptr(m_blocks[i][j]);
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
	    CoreOperator *op;
	    //Find the first term non null in the line
	    for (int i = 0; i < nrows; i++)
	    {
		op = Barman::get_CoreOperator_ptr(m_blocks[i][j]);
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


bool CoreOperator::checkSize(const CoreOperator &you) const
{
    std::string myType = WhatIsMyType();
    std::string yourType = you.WhatIsMyType();
    if(myType == "elementary" && yourType == "elementary")
	return true;
    if(myType == "node")
    {
	CoreOperator *op_left = m_operators.get_left();
	return op_left->checkSize(you);
    }
    if(yourType=="node")
    {
	CoreOperator *op_left = you.m_operators.get_left();
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
	    CoreOperator *mine_aux = Barman::get_CoreOperator_ptr(m_blocks[i][j]);
	    CoreOperator *yours_aux = Barman::get_CoreOperator_ptr(you.m_blocks[i][j]);
	    if(mine_aux == nullptr  || yours_aux == nullptr)
		continue;
	    bool res_aux = mine_aux->checkSize(*yours_aux);
	    if (res_aux == false)
		return false;
	}
    }
    return true;
}

std::string CoreOperator::WhatIsMyType() const
{
    std::string type = "elementary";
    if(m_operators.size() > 0)
	type = "node";
    if(m_blocks.size() > 0)
	type = "block";
    return type;
}

void CoreOperator::Print(bool isEnd){
    std::string type = WhatIsMyType();
    if(isEnd)
	Message::Info("I'm the CoreOperator %d, I'm a %s and here is my structure:", m_id, type.c_str());
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
		CoreOperator *op = Barman::get_CoreOperator_ptr(id);
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

void CoreOperator::PrintShape()
{
    std::vector<int> shape(2);
    getElementaryBlockSize(&shape);
    Message::Info("CoreOperator %d shape : (%d, %d) (in Elementary blocks)", m_id, shape[0], shape[1]);
}

bool compareSize(CoreOperator *op)
{


}


CoreOperator operator+(CoreOperator &lhs, const CoreOperator& rhs)
{
    Barman::Print();
    CoreOperator *res = new CoreOperator(true);
    //check size!
    bool checkS = lhs.checkSize(rhs);
    if(!checkS || lhs.m_dof != rhs.m_dof || lhs.m_trial != rhs.m_trial)
    {
	Message::Error("CoreOperator are not of the same block structure! result is crap...");
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

/*CoreOperator CoreOperator::operator=(CoreOperator rhs)
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

void CoreOperator::operator=(const CoreOperator& rhs)
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
