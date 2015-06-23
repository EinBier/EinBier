#include <Common/Type.h>

#include <Core/Operator.h>
#include <Core/BIO.h>

BIO::BIO(int r, int c, bool management) : Operator(r, c, management)
{
    m_shape = Shape(r, c);
    m_shape_block = Shape(1, 1);
}

BIO::BIO(Shape shape, bool management): Operator(shape, management)
{
    m_shape_block = Shape(1, 1);
}
