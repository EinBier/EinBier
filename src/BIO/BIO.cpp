#include <Common/Type.h>

#include <BIO/BIO.h>
#include <Core/Operator.h>

BIO::BIO(int r, int c)
{
	shape = Coord(r, c);
}

Operator BIO::create()
{
	Operator Op(shape);
	Op.Shape = Coord(1, 1);
	Op.bio = this;
	return Op;
}
