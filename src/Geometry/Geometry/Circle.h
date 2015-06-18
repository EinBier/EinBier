#pragma once

#include <Geometry/Geometry.h>
#include <vector>

class Circle : public Geometry
{
 private:
  double p_radius;
  std::vector<double> p_center;
 public:
  Circle(double radius, std::vector<double> center);
  Circle(double radius, double x, double y);
  bool Intersect(Circle *c);
  double GetRadius(){return p_radius;}
  void GetCenter(std::vector<double> *center);
  void Print();
};
