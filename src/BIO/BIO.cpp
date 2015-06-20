#include <Common/Type.h>

#include <BIO/BIO.h>
#include <Core/Operator.h>

BIO::BIO(int r, int c)
{
	shape.nrow = r;
	shape.ncol = c;
}

Operator BIO::create()
{
	Operator Op(shape.nrow, shape.ncol);
	Op.Shape.nrow = 1;
	Op.Shape.ncol = 1;
	Op.bio = this;
	return Op;
}
