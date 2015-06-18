#pragma once

#include <vector>
#include <list>


typedef struct{
    int row;
    int col;
} Coord;


class Operator
{
public:
    Coord shape;
    Coord Shape;

    Operator();
    Operator(Coord);
    Operator(int, int);

    void addBlock(int, int, Operator*);

    void Print();

private:
    Coord get_shape(){return shape;};
    Coord get_Shape(){return Shape;};

    // change with a new type but how ?
    /// so UGLY !!!
    std::list<int> rows;
    std::list<int> cols;
    std::list<Operator*> ops;

    // store the size of the banded block
    std::vector<int> banded_rows;
    std::vector<int> banded_cols;

    void _Operator(int, int);
    bool _isInList(int, std::list<int>);
    bool _isCheckAndUpdate_shapes(int, int, Coord);
};

