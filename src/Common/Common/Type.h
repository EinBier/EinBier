#ifndef TYPE_H
#define TYPE_H

#include <memory>

class Coord
{
public:
    int row;
    int col;
    Coord(int ii=0, int jj=0){row=ii; col=jj;};

    Coord & operator=(Coord other)
    {
        std::swap(row, other.row);
        std::swap(col, other.col);
        return *this;
    };
    int get_row(){return row;};
    int get_col(){return col;};
    void set_row(int ii){row=ii;};
    void set_col(int ii){col=ii;};
};

#endif
