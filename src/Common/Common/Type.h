#pragma once

#include <memory>

#define MyMin(a, b) ((a) < (b) ? (a) : (b))

class Coord
{
public:
    int row;
    int col;
    Coord(int ii=0, int jj=0){row=ii; col=jj;};

    // perhaps not useful because the compiler already provides an assign
    //// more or less T & operator=(const T &)
    Coord & operator=(Coord other)
    {
        std::swap(row, other.row);
        std::swap(col, other.col);
        return *this;
    };
    bool  operator==(Coord other)
    {
        if (this->get_row()==other.get_row()
            && this->get_col()==other.get_col()) {
            return true;
        } else {
            return false;
        }
    };
    bool  operator!=(Coord other)
    {
        if (this->get_row()!=other.get_row()
            || this->get_col()!=other.get_col()) {
            return true;
        } else {
            return false;
        }
    };
    bool operator<(Coord other)
    {
        if (this->get_row()<other.get_row()
            || this->get_col()<other.get_col()) {
            return true;
        } else {
            return false;
        }
    };

    int get_row(){return row;};
    int get_col(){return col;};
    void set_row(int ii){row=ii;};
    void set_col(int ii){col=ii;};
};
