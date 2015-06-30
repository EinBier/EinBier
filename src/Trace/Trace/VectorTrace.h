#pragma once

#include <Trace/Trace.h>
#include<string>
#include<vector>


class VectorTrace
{
private:
    std::vector<Trace *> m_traces;
    std::vector<VectorTrace *> m_vtraces;

public:
    VectorTrace(){m_traces.resize(0); m_vtraces.resize(0);}
    ~VectorTrace();
    void push_back(Trace *t);
    void push_back(VectorTrace *t);
    void Print(bool isEnd = true);
    int get_number_of_Trace();
};
