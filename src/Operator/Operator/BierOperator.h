#pragma once

#include<Common/Shape.h>
#include<Barman/Bier.h>
#include<Trace/Trace.h>

#include<vector>
#include<string>


class BierOperator : public Bier {

    class Node;

protected:
    std::string m_name;

private:
    Trace* m_test;
    Trace* m_trial;

    Shape m_Shape;

    // offset starts at 0
    // and finishes at Number Of Elementary Traces
    std::vector<int> m_offset_test;
    std::vector<int> m_offset_trial;

    bool m_del_test;
    bool m_del_trial;

public:
    static std::string ELEMENTARY;
    static std::string BLOCK;
    static std::string BELEM;
    static std::string UNDEFINED;

    static std::string UNARY;
    static std::string BINARY;


    BierOperator(std::string name = "default");
    ~BierOperator();

    void Print();

    Trace getSubTrace(Trace*, int, std::vector<int>);
    Trace* getTraceTest() {return m_test;}
    Trace* getTraceTrial() {return m_trial;}

    void setTrace(Trace*, Trace*);
    void setBlock(int, int, BierOperator *);
    // getBlock(int, int)

    void setShape(Shape);
    Shape getShape();
    Shape getSize();

};
