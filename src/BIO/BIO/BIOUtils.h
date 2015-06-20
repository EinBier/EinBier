#include <BIO/BIO.h>

class BIOVal : public BIO
{
public:
	double val;
	BIOVal(int r=0, int c=0, double v=42.);
	virtual double getValue(int, int);
};


class BIOZero : public BIOVal
{
public:
	BIOZero(int r, int c);
	virtual double getValue(int, int);
};

class BIOEye : public BIO
{
public:
	BIOEye(int r=0, int c=0);
	virtual double getValue(int, int);
};
