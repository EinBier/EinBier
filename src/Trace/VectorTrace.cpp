#include<Common/Message.h>
#include<Trace/VectorTrace.h>
#include<vector>
#include<string>
#include<iostream>

VectorTrace::~VectorTrace()
{
    int nt = m_traces.size();
    int nvt = m_vtraces.size();
    for (int i = nt-1; i >= 0; i --)
    {
//	delete m_traces[i];
    }
    for (int i = nvt-1; i >= 0; i --)
    {
//	delete m_vtraces[i];
    }
}

void VectorTrace::Print(bool isEnd)
{
    int nt = m_traces.size();
    int nvt = m_vtraces.size();
    std::cout<<"[";
    for (int i = 0; i < nt; i ++)
    {
	std::cout << m_traces[i]->TreePrint();
	if(i < nt-1)
	    std::cout << ", ";
    }
    if(nvt > 0)
    {
	for (int i = 0; i < nvt; i ++)
	{
	    m_vtraces[i]->Print(false);
	    if(i < nvt-1)
		std::cout << ", ";
	}
    }    
    std::cout<<"]";
    if(isEnd)
	std::cout << std::endl;
 }

void VectorTrace::push_back(Trace *t){
    int nt = m_traces.size();
    int nvt = m_vtraces.size();
    if(nvt > 0)
    {
	Message::Error("This VectorTrace contains other VectorTraces!");
	return;
    }
    m_traces.push_back(t);
}


void VectorTrace::push_back(VectorTrace *vt){
    int nt = m_traces.size();
    int nvt = m_vtraces.size();
    if(nt > 0)
    {
	Message::Error("This VectorTrace contains other Traces!");
	return;
    }
    m_vtraces.push_back(vt);
    
}


int VectorTrace::get_number_of_Trace(){
    int nt = m_traces.size();
    int nvt = m_vtraces.size();
    if(nt > 0)
	return nt;
    int n = 0;
    for (int i = 0; i < nvt; i ++)
	n += m_vtraces[i]->get_number_of_Trace();
    return n;
}
