#include <Common/Message.h>

#include <BIO/BIO.h>

#include <Core/Node.h>

#include <Core/Operator.h>


void Operator::createOperator(int r, int c)
{
    shape = Coord(r, c);
    Shape.row = 0;
    Shape.col = 0;
    banded_rows.push_back(-1);
    banded_cols.push_back(-1);
    Message::Info("Operator created. [%p]", this);
    return;
}

Operator::Operator(int r, int c)
{
    createOperator(r, c);
}

Operator::Operator(Coord ij)
{
    createOperator(ij.row, ij.col);
}

void Operator::addBlock(Coord ij, Operator *op)
{
	addBlock(ij.row, ij.col, op);
	return;
}

void Operator::addBlock(int r, int c, Operator *op)
{
	if (isInList(r, rows) && isInList(c, cols)) {
		Message::Warning("occupied!");
		return;
	} else if (!isCheckAndUpdate_shapes(r, c, op->get_shape())) {
		Message::Error("wrong size!");
		return;
	} else {
		rows.push_back(r);
		cols.push_back(c);
		ops.push_back(op);
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

bool Operator::isCheckAndUpdate_shapes(int r, int c, Coord coord)
{
	int rr = coord.get_row();
	int cc = coord.get_col();

	if (banded_rows.size() < r)
		banded_rows.resize(r, -1);
	if (banded_cols.size() < c)
		banded_cols.resize(c, -1);

	if (banded_rows[r] == -1) {
		banded_rows.insert(banded_rows.begin()+r, rr);
	} else {
		if (banded_rows[r] != rr) {
			Message::Info("Already blocks at row:%d [shape.row:%d !=%d]", r, rr,
						  banded_rows[r]);
			return false;
		} else {
			Message::Info("Already blocks at row:%d [shape.row:%d ok]", r, rr);
		}
	}

	if (banded_cols[c] == -1) {
		banded_cols.insert(banded_cols.begin()+c, cc);
	} else {
		if (banded_cols[c] != cc) {
			Message::Info("Already blocks at col:%d [shape.col:%d !=%d]", c, cc,
						  banded_cols[c]);
			return false;
		} else {
			Message::Info("Already blocks at col:%d [shape.col:%d ok]", c, cc);
		}
	}
	if (Shape.get_row() <= r)
		Shape.set_row(r+1);
	if (Shape.get_col() <= c)
		Shape.set_col(c+1);

	std::vector<int>::iterator itr;
	int sumr = 1;
	for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr) {
		sumr += *itr;
	}
	int sumc = 1;
	for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr) {
		sumc += *itr;
	}
	shape = Coord(sumr, sumc);
	return true;
}

Matrix Operator::assemb()
{
    int r = 0, c = 0;
    if (Shape == Coord(0, 0)) {
	Message::Error("No BIO attached.");
	return Matrix(Coord(0, 0));
	}
    else if (Shape==Coord(1, 1)) {
        Message::Info("Operator assembling... [%p]", this);
        Matrix *m = new Matrix(shape);
        for (r=0; r<shape.get_row(); r++) {
            for (c=0; c<shape.get_col(); c++) {
                m->insert(r, c, getValue(r, c));
            }
        }
		Message::Info("Operator assembled -> return Matrix. [%p] -> [%p]", this, m);
		return *m;
    } else if (Shape < Coord(0, 0)) {
	Message::Info("Operator composed: %d(+) and %d(*)", -Shape.get_row(), -Shape.get_col());
	std::string op = node.m_op;
	Matrix L = (node.m_left)->assemb();
	if (op == "+") {
	    Matrix R = (node.m_right)->assemb();
	    return L + R;
	} else if (op == "*") {
	    return L * (*node.m_scalar);
	}
    } else {
        return Matrix(Coord(0, 0));
    }
}

Operator Operator::operator+(Operator & other)
{
	Operator *tmp = new Operator(shape);
	if (shape == other.shape) {
		int nleftAdd = 0;
		int nrightAdd = 0;
		if (Shape.get_row()<0)
			nleftAdd = -Shape.get_row();
		if (other.Shape.get_row()<0)
			nrightAdd = -other.Shape.get_row();
		tmp->Shape.set_row(-(nleftAdd + nrightAdd +1));
		tmp->node.compute("+", this, &other);
	} else {
		Message::Error("Addition impossible, wrong shape (%d,%d)(%d,%d)",
					   shape.row, shape.col,
					   other.shape.row, other.shape.col);
		Message::Error("addition : [%p]	 +	[%p]  failed!", this, &other);
		*tmp = 0;
	}
	return *tmp;
}

Operator Operator::operator*(double v)
{
	Operator *tmp = new Operator(shape);
	if (Shape.get_col()<0){
		tmp->Shape.set_col(Shape.get_col()-1);
	} else {
		tmp->Shape.set_col(-1);
	}
	tmp->node.compute("*", this, &v);
	return *tmp;
}

//
void Operator::Print()
{
	Message::Info("Operator: \tshape: %d %d\tShape: %d %d \t [%p]",
				  shape.row, shape.col,
				  Shape.row, Shape.col,
				  this);
	return;
}
//
