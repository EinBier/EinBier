#include<Common/Message.h>
#include <Operator.h>



void Operator::createOperator(int row, int col, bool management)
{
    m_shape = Shape(row, col);
    m_shape_block = Shape(0, 0);
    m_banded_rows.push_back(-1);
    m_banded_cols.push_back(-1);

    Barman *opH;
    opH = opH->getBarman();
    if(opH == nullptr) {
        Message::Error("Operator::createOperator: Init the Barman first!");
        return;
    }
    m_id = opH->addOperator(this, management);
    if(m_id >= 0) {
        Message::Debug("Operator created.     (barman: %d) (id: %d)   [%p]", opH, m_id, this);
    } else {
        Message::Error("Operator could not be created. [%p] (id: %d)", this, m_id);
    }
    return;
}

Operator::Operator(int row, int col, bool management)
{
    Message::Debug("Operator by row col");
    createOperator(row, col, management);
}

Operator::Operator(Shape shape, bool management)
{
    Message::Debug("Operator by Shape");
    createOperator(shape.get_row(), shape.get_col(), management);
}


Operator::~Operator()
{
    Barman *opH;
    opH = opH->getBarman();
    if(opH == NULL)
        return;
    int check = opH->removeOperator(m_id);
    Message::Debug("Operator removed.     (barman: %d) (id: %d) (status: %d)",
                   opH, m_id, check);
}


void Operator::setBlockSize(int nrow, int ncol){
    m_blocks.resize(n);
    for (int i = 0; i < n; i++)
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
    m_blocks[k][l] = op->getId();
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
	setSize(dof->getBlockSize(), trial->getBlockSize());
    m_dof = dof;
    m_trial = trial;

}
