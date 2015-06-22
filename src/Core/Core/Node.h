#pragma once

#include <string>
//#include <Core/Operator.h>
class Operator;

class Node
{
public:
	std::string m_op;
	Operator *m_left;
	Operator *m_right;
	double *m_scalar;
Node(std::string s="", Operator *l=0, Operator *r=0, double *v=0):m_op(s), m_left(l), m_right(r), m_scalar(v)
	{};
	void compute(std::string s, Operator *l, Operator *r){m_op=s; m_left=l; m_right=r;};
	void compute(std::string s, Operator *l, double *v){m_op=s; m_left=l; m_scalar=v;};
};
