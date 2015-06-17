#pragma once

#include <Geometry/Geometry.h>
#include <vector>

class Circle : public Geometry
{
 private:
  double m_radius;
  std::vector<double> m_center;
 public:
  Circle(double radius, std::vector<double> center);
  Circle(double radius, double x, double y);
  bool Intersect(Circle *c);
  double GetRadius(){return m_radius;}
  void GetCenter(std::vector<double> *center);
  void Print();
};
