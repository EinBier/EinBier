#pragma once

#include <Core/Operator.h>

class OperatorVal : public Operator
{
public:
	double val;
	OperatorVal(int r=0, int c=0, double v=42.);
	double getValue(int, int);
};


class OperatorZero : public OperatorVal
{
public:
	OperatorZero(int r, int c);
	double getValue(int, int);
};

class OperatorEye : public Operator
{
public:
	OperatorEye(int r=0, int c=0);
	double getValue(int, int);
};
