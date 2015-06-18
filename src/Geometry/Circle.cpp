#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>
#include <Common/Message.h>

#include <stdlib.h>
#include <math.h>

Circle::Circle(double radius, double x, double y):p_radius(radius)
{
  p_center.resize(2);
  p_center[0]=(x);
  p_center[1]=(y);
}


Circle::Circle(double radius, std::vector<double> center):p_radius(radius)
{
  if(center.size() != 2)
    {
      Message::Error("The center of the disk must be a 2D vector! Abording...");
      Message::Finalize(EXIT_FAILURE);
    }
  p_center.resize(2);
  for (int i = 0; i < 2; i ++)
    p_center[i] = center[i];
}

void Circle::GetCenter(std::vector<double> *center)
{
  center->resize(2);
  (*center)[0]=p_center[0];
  (*center)[1]=p_center[1];
}


//Equal to true if the two circles intersect, false otherwise
bool Circle::Intersect(Circle *c)
{
  double radius = c->GetRadius();
  std::vector<double> center(2);
  c->GetCenter(&center);
  double d = (p_center[0]-center[0])*(p_center[0]-center[0]) + (p_center[1]-center[1])*(p_center[1]-center[1]);
  d = sqrt(d);
  return (d < (radius + p_radius));
}

//Print stat of the circle
void Circle::Print()
{
  Message::Info("Circle of radius %g, and center (%g,%g)\n", p_radius, p_center[0], p_center[1]);
  return;
}
