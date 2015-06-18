#pragma once

#include <Geometry/Geometry.h>
#include <Geometry/Ellipse.h>
#include <vector>

class Circle : public Ellipse
{
  private:
   double p_radius;
  public:
   Circle(double radius, std::vector<double> center);
   Circle(double radius, double x, double y);
//bool Intersect(Circle *c);
   double GetRadius(){return p_radius;}
   void GetCenter(std::vector<double> *center);
   void Print();
};
