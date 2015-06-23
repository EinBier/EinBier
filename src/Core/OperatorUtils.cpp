
#include <Core/Operator.h>
#include <Core/OperatorUtils.h>

OperatorVal::OperatorVal(int r, int c, double v) : Operator(r, c), val(v)
{
    Shape = Coord(1, 1);
}

double OperatorVal::getValue(int, int)
{
    return val;
}

OperatorZero::OperatorZero(int r, int c) : OperatorVal(r, c, 0.)
{
    // weird C++
}
double OperatorZero::getValue(int, int)
{
    return 0.;
}

OperatorEye::OperatorEye(int r, int c) : Operator(r, c)
{
    Shape = Coord(1, 1);
}
double OperatorEye::getValue(int i, int j)
{
    if (i == j) {
    return 1.;
    } else {
    return 0.;
    }
}
