#pragma once

#include <Common/Type.h>

#include <Core/Operator.h>

class BIO : public Operator
{
public:
	BIO(int r=0, int c=0);
//	BIO(Trace, Trace);

	virtual double getValue(int, int){}; // do not forget {}, so weird c++ !!
};
