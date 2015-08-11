#pragma once

class Shape
{
private:
    int m_row;
    int m_col;

public:
Shape():Shape(0,0){}
Shape(int row, int col)
    : m_row(row),  m_col(col) {};
    
    bool operator==(Shape other)
    {
        return (m_row==other.get_row() && m_col==other.get_col());
    }

    bool operator!=(Shape other)
    {
        return (m_row != other.get_row() || m_col != other.get_col());
    }

    bool operator<(Shape other)
    {
        if(m_row > other.get_row())
            return false;
        else if(m_row < other.get_row())
            return true;
        else //Equality, check the column
            return (m_col < other.get_col());
    }
    void set(int row, int col){m_row = row; m_col = col;}
    int get_row(){return m_row;}
    int get_col(){return m_col;}
    void set_row(int row){m_row = row;}
    void set_col(int col){m_col = col;}
};
