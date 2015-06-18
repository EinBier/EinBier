#include <Common/Message.h>

#include <Core/Operator.h>

Operator::Operator()
{
    _Operator(0, 0);
}
Operator::Operator(Coord s)
{
    _Operator(s.row, s.col);
}
Operator::Operator(int r, int c)
{
    _Operator(r, c);
}
void Operator::_Operator(int r, int c)
{
    shape.row = r;
    shape.col = c;
    Shape.row = 0;
    Shape.col = 0;
    banded_rows.push_back(-1);
    banded_cols.push_back(-1);
    return;
}
 
void Operator::addBlock(int r, int c, Operator *op)
{
    if (_isInList(r, rows) && _isInList(c, cols))
    {
        Message::Error("occupied!");
        return;
    }
    else
    {
        if (!_isCheckAndUpdate_shapes(r, c, op->get_shape()))
        {
            Message::Error("wrong size!");
            return;
        }
        rows.push_back(r);
        cols.push_back(c);
        ops.push_back(op);
    }
    return;
}

bool Operator::_isInList(int i, std::list<int> l)
{
    std::list<int>::iterator itr;
    for(itr=l.begin(); itr!=l.end(); itr++)
    {
        if (i == *itr)
        {
            return true;
        }
    }
    return false;
}

bool Operator::_isCheckAndUpdate_shapes(int r, int c, Coord coord)
{
    int rr = coord.row;
    int cc = coord.col;

    if (banded_rows.size() < r)
    {
        banded_rows.resize(r, -1);
    }
    if (banded_cols.size() < c)
    {
        banded_cols.resize(c, -1);
    }

    if (banded_rows[r] == -1)
    {
        banded_rows.insert(banded_rows.begin()+r, rr);
    }
    else
    {
        if (banded_rows[r] != rr)
        {
            return false;
        }
    }
    if (banded_cols[c] == -1)
    {
        banded_cols.insert(banded_cols.begin()+c, cc);
    }
    else
    {
        if (banded_cols[c] != cc)
        {
            return false;
        }
    }
    if (Shape.row < r)
    {
        Shape.row = r;
    }
    if (Shape.col < c)
    {
        Shape.col = c;
    }

    std::vector<int>::iterator itr;
    int sumr = 1;
    for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr)
    {
        sumr += *itr;
    }
    int sumc = 1;
    for(itr=banded_rows.begin(); itr!=banded_rows.end(); ++itr)
    {
        sumc += *itr;
    }
    shape.row = sumr;
    shape.col = sumc;
    return true;
}

//
void Operator::Print()
{
  Message::Warning("Print");
  Message::Info("shape: %d %d", shape.row, shape.col);
  Message::Info("Shape: %d %d", Shape.row, Shape.col);
  return;
}
