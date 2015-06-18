#pragma once

#include <Geometry/Geometry.h>
#include <vector>

class Ellipse : public Geometry
{
 protected:
  double p_xradius;
  double p_yradius;
  std::vector<double> p_center;
 public:
  Ellipse(double xradius, double yradius, std::vector<double> center);
  Ellipse(double xradius, double yradius, double x, double y);
  bool Intersect(Ellipse *e);
  double GetXRadius(){return p_xradius;}
  double GetYRadius(){return p_yradius;}
  void GetCenter(std::vector<double> *center);
  void setParam(double xradius, double yradius, double x, double y);
  virtual void Print();
};
