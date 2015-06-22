
#include <BIO/BIO.h>
#include <BIO/BIOUtils.h>

BIOVal::BIOVal(int r, int c, double v) : BIO(r, c), val(v)
{
    // weird C++
}

double BIOVal::getValue(int, int)
{
    return val;
}

BIOZero::BIOZero(int r, int c) : BIOVal(r, c, 0.)
{
    // weird C++
}
double BIOZero::getValue(int, int)
{
    return 0.;
}

BIOEye::BIOEye(int r, int c) : BIO(r, c)
{
    // weird C++
}
double BIOEye::getValue(int i, int j)
{
    if (i == j) {
	return 1.;
    } else {
	return 0.;
    }
}

