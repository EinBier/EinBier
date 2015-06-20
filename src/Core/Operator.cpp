#include <Common/Message.h>

//#include <BIO/BIO.h>
#include <Core/Operator.h>

Operator::Operator(int r, int c)
{
	shape.nrow = r;
	shape.ncol = c;
	Shape.nrow = 0;
	Shape.ncol = 0;
	banded_rows.push_back(-1);
	banded_cols.push_back(-1);
//	bio = 0;
}

Operator::Operator(Coord s)
{
  Operator(s.nrow, s.ncol);
}

void Operator::addBlock(int r, int c, BIO *bio)
{
	Operator op = bio->create();
	addBlock(r, c, &op);
	return;
}


void Operator::addBlock(int r, int c, Operator *op)
{
	if (isInList(r, rows) && isInList(c, cols)) {
		Message::Error("occupied!");
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
	int rr = coord.nrow;
	int cc = coord.ncol;

	if (banded_rows.size() < r)
		banded_rows.resize(r, -1);
	if (banded_cols.size() < c)
		banded_cols.resize(c, -1);

	if (banded_rows[r] == -1) {
		banded_rows.insert(banded_rows.begin()+r, rr);
	} else {
		if (banded_rows[r] != rr) {
			return false;
		} else {
			Message::Error("Panic !! impossible case");
		}
	}

	if (banded_cols[c] == -1) {
		banded_cols.insert(banded_cols.begin()+c, cc);
	} else {
		if (banded_cols[c] != cc) {
			return false;
		} else {
			Message::Error("Panic !! impossible case");
		}
	}
	if (Shape.nrow <= r)
		Shape.nrow = r+1;
	if (Shape.ncol <= c)
		Shape.ncol = c+1;

	std::vector<int>::iterator itr;
	int sumr = 1;
	for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr) {
		sumr += *itr;
	}
	int sumc = 1;
	for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr) {
		sumc += *itr;
	}
	shape.nrow = sumr;
	shape.ncol = sumc;
	return true;
}

//
void Operator::Print()
{
	Message::Info("shape: %d %d", shape.nrow, shape.ncol);
	Message::Info("Shape: %d %d", Shape.nrow, Shape.ncol);
	return;
}

