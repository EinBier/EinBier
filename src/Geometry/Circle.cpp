#include <Geometry/Geometry.h>
#include <Geometry/Ellipse.h>
#include <Geometry/Circle.h>
#include <Common/Message.h>

#include <stdlib.h>
#include <math.h>

Circle::Circle(double radius, double x, double y):Ellipse(radius, radius, x, y)
{
  p_radius = p_xradius;
}


Circle::Circle(double radius, std::vector<double> center):Ellipse(radius, radius, center)
{
  p_radius = p_xradius;
}

//Print stat of the circle
void Circle::Print()
{
  Message::Info("Circle of radius %g, and center (%g,%g)\n", p_radius, p_center[0], p_center[1]);
  return;
}
