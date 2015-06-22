#ifndef BIO_H
#define BIO_H

#pragma once

#include <Common/Type.h>

//#include <Core/Operator.h>
class Operator; // to prevent circle hell dependencies

class BIO
{
public:
	Coord shape;
	BIO(int r=0, int c=0);

	Operator create();
	virtual double getValue(int, int){}; // do not forget {}, so weird c++ !!
};

#endif
