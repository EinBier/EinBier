#pragma once

#include<Common/Message.h>

#include<string>


class Bier
{
protected:
    int m_id;

public:
    virtual ~Bier();
    Bier(bool in_charge = false);
    int getId(){return m_id;}
};
