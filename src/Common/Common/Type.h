#pragma once

class Shape
{
private:
    int m_row;
    int m_col;

public:
    Shape(int row=0, int col=0)
        : m_row(row), m_col(col){};
    ~Shape(){};

    // perhaps not useful because the compiler already provides an assign
    //// more or less T & operator=(const T &)
    Shape & operator=(Shape other)
    {
        m_row = other.get_row();
        m_col = other.get_col();
        return *this;
    };

    bool operator==(Shape other)
    {
        return (m_row==other.get_row() && m_col==other.get_col());
    };

    bool operator!=(Shape other)
    {
        return (m_row != other.get_row() || m_col != other.get_col());
    };

    bool operator<(Shape other)
    {
        if(m_row > other.get_row())
            return false;
        else if(m_row < other.get_row())
            return true;
        else //Equality, check the column
            return (m_col < other.get_col());
    };

    int get_row(){return m_row;};
    int get_col(){return m_col;};
    void set_row(int row){m_row = row;};
    void set_col(int col){m_col = col;};
};
