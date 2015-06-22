#ifndef NODE_H
#define NODE_H

#include <string>
//#include <Core/Operator.h>
class Operator;

class Node
{
public:
	std::string op;
	Operator *left;
	Operator *right;
	double *scalar;
	Node(std::string s="", Operator *l=0, Operator *r=0, double *v=0)
	{op = s; left = l; right = r; scalar = v;};
	void compute(std::string s, Operator *l, Operator *r){op=s;left=l;right=r;};
	void compute(std::string s, Operator *l, double *v){op=s;left=l;scalar=v;};
};

#endif
