#pragma once

#include <stdexcept>

class Shape {

private:
    int m_row;
    int m_col;

public:
    int a;

    Shape() : Shape(0,0) {};
    Shape(int row, int col) : m_row(row),  m_col(col) {};

    void set(int row, int col){m_row = row; m_col = col;}

    void set_row(int row){m_row = row;}
    void set_col(int col){m_col = col;}

    // should disappear ?
    // because it is better to have only one method
    // to access, and [] seems more readable.
    int get_row(){return m_row;}
    int get_col(){return m_col;}

    bool operator==(Shape other) {
        return (m_row==other.get_row() && m_col==other.get_col());
    }

    bool operator!=(Shape other) {
        return (m_row != other.get_row() || m_col != other.get_col());
    }

    bool operator<(Shape other) {
        if(m_row > other.get_row())
            return false;
        else if(m_row < other.get_row())
            return true;
        else //Equality, check the column
            return (m_col < other.get_col());
    }

    int operator[](int i) {
        if (i == 0)
            return get_row();
        else if (i == 1)
            return get_col();
        else {
            std::runtime_error("Ouch!");
            return -42;
        // WARNING: nothing to prevent it is wrong. Add Message:: ?
        }
    }

};
