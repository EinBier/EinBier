#include <Common/Message.h>
#include <Common/Type.h>

#include <Core/OperatorHandler.h>
#include <Core/Operator.h>

#include<string>
#include<vector>
#include<list>

void Operator::createOperator(int row, int col, bool management)
{
    m_shape = Shape(row, col);
    m_shape_block.set_row(0);
    m_shape_block.set_col(0);
    m_banded_rows.push_back(-1);
    m_banded_cols.push_back(-1);
//OPERATOR HANDLER
    OperatorHandler *opH;
    opH = opH->getOperatorHandler();
    int id = opH->addOperator(this, management);
    if(id>=0)
	Message::Info("Operator created. [%p]", this);
    else
	Message::Error("Operator could not be created. [%p]", this);
    m_id = id;
    Message::Debug("id = %d", id);
    return;
}

Operator::Operator(int row, int col, bool management)
{
    createOperator(row, col, management);
}

Operator::Operator(Shape shape, bool management)
{
    createOperator(shape.get_row(), shape.get_col(), management);
}


//DESTRUCTOR: Modifier le 
Operator::~Operator()
{
    OperatorHandler *opH;
    opH = opH->getOperatorHandler();
    if(opH != NULL)
	//The operator must be delete from the OperatorHandler
	opH->removeOperator(m_id);
}


void Operator::setBlock(Shape shape, Operator *op)
{
    setBlock(shape.get_row(), shape.get_col(), op);
    return;
}

void Operator::setBlock(int row, int col, Operator *op)
{
    if (isInList(row, m_rows) && isInList(col, m_cols)) {
	Message::Warning("Operator::setBlock: the place (%d,%d) is already occupied!", row,col);
	return;
    } else if (!isCheckAndUpdate_shapes(row, col, op->get_shape())) {
	Message::Error("Operator::setBlock: wrong size!");
	return;
    } else {
	m_rows.push_back(row);
	m_cols.push_back(col);
	m_ops.push_back(op);
    }
    return;
}

bool Operator::isInList(int i, std::list<int> l)
{
    std::list<int>::iterator itr;
    for(itr=l.begin(); itr!=l.end(); itr++) {
        if (i == *itr)
            return true;
    }
    return false;
}

bool Operator::isCheckAndUpdate_shapes(int r, int c, Shape shape)
{
    int rr = shape.get_row();
    int cc = shape.get_col();

    if (m_banded_rows.size() < r)
        m_banded_rows.resize(r, -1);
    if (m_banded_cols.size() < c)
        m_banded_cols.resize(c, -1);

    if (m_banded_rows[r] == -1) {
        m_banded_rows.insert(m_banded_rows.begin()+r, rr);
    } else {
        if (m_banded_rows[r] != rr) {
            Message::Info("Already blocks at row:%d [shape.row:%d !=%d]", r, rr,
                          m_banded_rows[r]);
            return false;
        } else {
            Message::Info("Already blocks at row:%d [shape.row:%d ok]", r, rr);
        }
    }

    if (m_banded_cols[c] == -1) {
        m_banded_cols.insert(m_banded_cols.begin()+c, cc);
    } else {
        if (m_banded_cols[c] != cc) {
            Message::Info("Already blocks at col:%d [shape.col:%d !=%d]", c, cc,
                          m_banded_cols[c]);
            return false;
        } else {
            Message::Info("Already blocks at col:%d [shape.col:%d ok]", c, cc);
        }
    }

    if (m_shape_block.get_row() <= r)
        m_shape_block.set_row(r+1);
    if (m_shape_block.get_col() <= c)
        m_shape_block.set_col(c+1);

    std::vector<int>::iterator itr;
    int sumr = 1;
    for(itr=m_banded_rows.begin(); itr!=m_banded_rows.end(); ++itr) {
        sumr += *itr;
    }
    int sumc = 1;
    for(itr=m_banded_rows.begin(); itr!=m_banded_rows.end(); ++itr) {
        sumc += *itr;
    }
    shape = Shape(sumr, sumc);
    return true;
}

Matrix Operator::assemble()
{
    if (m_shape_block == Shape(0, 0)) {
	Message::Error("No BIO attached.");
	return Matrix(Shape(0, 0));
    }
    else if (m_shape_block==Shape(1, 1)) {
        Message::Info("Operator assembling... [%p]", this);
        Matrix *m = new Matrix(m_shape);
        for (int r=0; r < m_shape.get_row(); r++) {
            for (int c=0; c < m_shape.get_col(); c++) {
                m->insert(r, c, getValue(r, c));
            }
        }
        Message::Info("Operator assembled -> return Matrix. [%p] -> [%p]", this, m);
        return *m;
    }
    else if (m_shape_block < Shape(0, 0)) {
	Message::Info("Operator composed: %d(+) and %d(*)", -m_shape_block.get_row(), -m_shape_block.get_col());
	std::string op = node.m_op;
	Matrix L = (node.m_left)->assemble();
	if (op == "+") {
	    Matrix R = (node.m_right)->assemble();
	    return L + R;
	} else if (op == "*") {
	    return L * (*node.m_scalar);
	}
    }
    else
      return Matrix(Shape(0, 0));
}

Operator Operator::operator+(Operator & other)
{
    //The OperatorHandler is asked to manage this buddy
    Operator *tmp = new Operator(m_shape, true);
    if (m_shape == other.get_shape()) {
	int nleftAdd = 0;
	int nrightAdd = 0;
	if (m_shape_block.get_row()<0)
	    nleftAdd = -m_shape_block.get_row();
	if (other.m_shape_block.get_row()<0)
	    nrightAdd = -other.get_shape_block().get_row();
	tmp->get_shape_block().set_row(-(nleftAdd + nrightAdd +1));
	tmp->node.set("+", this, &other);
    } else {
	Message::Error("Addition impossible, wrong shape (%d,%d)(%d,%d)",
		       m_shape.get_row(), m_shape.get_col(),
		       other.get_shape().get_row(), other.get_shape().get_col());
    }
    return *tmp;
}

Operator Operator::operator*(double v)
{
    //The OperatorHandler is asked to manage this buddy
    Operator *tmp = new Operator(m_shape, true);
    if (m_shape_block.get_col()<0){
	tmp.m_shape_block.set_col(m_shape_block.get_col()-1);
    } else {
	tmp.m_shape_block.set_col(-1);
    }
    tmp.node.compute("*", this, &v);
    return *tmp;
}


void Operator::Print()
{
    Message::Info("Operator: \tshape: %d %d\tShape: %d %d [%p]",
		  m_shape.get_row(), m_shape.get_col(),
		  m_shape_block.get_row(), m_shape_block.get_col(),
		  this);
    return;
}

