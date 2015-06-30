#include<Common/Message.h>

#include<Trace/Trace.h>

#include<string>

void Trace::Print()
{
    Message::Info("Trace : %d", m_n);
}

std::string Trace::TreePrint()
{
    return m_name;
}
