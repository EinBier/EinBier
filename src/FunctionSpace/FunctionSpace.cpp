#include<Common/Type.h>
#include<FunctionSpace/FunctionSpace.h>
#include<Trace/Trace.h>

#include<vector>
#include<string>


Scalar Dummy::Id(int i, int j, Trace *test)
{
    if (i == j)
        return 1.;
    else
        return 0.;

}
