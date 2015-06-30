#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>
#include<iostream>

void Trace::createTrace(int n, std::string name){
    m_n = n; 
    m_name = name;
    m_traces.resize(0);
}


void Trace::Print(bool isEnd)
{
    if(isElementary())
    {
	if(isEnd)
	    std::cout<<"[";
	std::cout << m_name;
	if(isEnd)
	    std::cout<<"]";
    }
    else
    {
	std::cout<<"[";
	int nt = m_traces.size();
	for (int i = 0; i < nt; i ++)
	{
	    m_traces[i]->Print(false);
	    if(i < nt-1)
		std::cout << ", ";
	}
	std::cout<<"]";
    }
     if(isEnd)
	 std::cout << std::endl;
}


void Trace::push_back(Trace *t){
    if(isElementary())
    {
	Message::Error("This Trace is Elementary!");
	return;
    }
    m_traces.push_back(t);
}


