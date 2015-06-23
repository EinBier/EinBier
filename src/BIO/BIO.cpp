#include <Common/Type.h>

#include <BIO/BIO.h>

BIO::BIO(int r, int c) : Operator(r, c)
{
    shape = Coord(r, c);
    Shape = Coord(1, 1);
}
