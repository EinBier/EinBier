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

    OperatorHandler *opH;
    opH = opH->getOperatorHandler();
    m_id = opH->addOperator(this, management);
    if(m_id >= 0) {
        Message::Debug("Operator created. [%p] (id: %d)", this, m_id);
    } else {
        Message::Error("Operator could not be created. [%p] (id: %d)", this, m_id);
    }
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


Operator::~Operator()
{
    OperatorHandler *opH;
    opH = opH->getOperatorHandler();
    if(opH != NULL)
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
        Message::Warning("Operator::setBlock already occupied! (%d,%d)", row,col);
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
            Message::Debug("Already blocks at row:%d [shape.row:%d !=%d]", r, rr,
                          m_banded_rows[r]);
            return false;
        } else {
            Message::Debug("Already blocks at row:%d [shape.row:%d ok]", r, rr);
        }
    }

    if (m_banded_cols[c] == -1) {
        m_banded_cols.insert(m_banded_cols.begin()+c, cc);
    } else {
        if (m_banded_cols[c] != cc) {
            Message::Debug("Already blocks at col:%d [shape.col:%d !=%d]", c, cc,
                          m_banded_cols[c]);
            return false;
        } else {
            Message::Debug("Already blocks at col:%d [shape.col:%d ok]", c, cc);
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
        Message::Debug("Operator assembling... [%p]", this);
        Matrix *m = new Matrix(m_shape);
        for (int r=0; r<m_shape.get_row(); r++) {
            for (int c=0; c<m_shape.get_col(); c++) {
                m->insert(r, c, getValue(r, c));
            }
        }
        Message::Debug("Operator::assemble returns Matrix: [%p] -> [%p]", this, m);
        return *m;
    }
    else if (m_shape_block < Shape(0, 0)) {
        Message::Debug("Operator composed: %d(+) and %d(*)",
                      -m_shape_block.get_row(), -m_shape_block.get_col());
        std::string op = m_node.m_op;
        Matrix L = (m_node.m_left)->assemble();
        if (op == "+") {
            Matrix R = (m_node.m_right)->assemble();
            return L + R;
        } else if (op == "*") {
            return L * (*m_node.m_scalar);
        }
    } else {
        return Matrix(Shape(0, 0));
    }
}

Operator Operator::operator+(Operator & other)
{
    Shape mime = get_shape_block();
    Shape their = other.get_shape_block();

    Operator *tmp = new Operator(get_shape(), true);

    if (get_shape() == other.get_shape()) {
        int nmul = 0;
        if (mime.get_col()<0)
            nmul += mime.get_col();
        if (their.get_col()<0)
            nmul += their.get_col();
        int update = -1;
        if (mime.get_row()<0)
            update += mime.get_row();
        if (their.get_row()<0)
            update += their.get_row();
        tmp->set_shape_block(Shape(update, nmul));
        tmp->m_node.set("+", this, &other);
    } else {
        Message::Error("Addition impossible, wrong shape (%d,%d)(%d,%d)",
                       mime.get_row(), mime.get_col(),
                       their.get_row(), their.get_col());
        Message::Error("addition : [%p] + [%p] failed!", this, &other);
        *tmp = 0;
    }
    return *tmp;
}

Operator Operator::operator*(double v)
{
    Shape mime = get_shape_block();

    Operator *tmp = new Operator(get_shape(), true);

    int update = -1;
    if (mime.get_col()<0)
        update += mime.get_col();
    tmp->set_shape_block(Shape(mime.get_row(), update));
    tmp->get_node_ptr()->set("*", this, &v);
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
