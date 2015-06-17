#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>
#include <Common/Message.h>

#include <stdlib.h>

Circle::Circle(double radius, double x, double y):m_radius(radius)
{
  m_center.resize(2);
  m_center[0]=(x);
  m_center[1]=(y);
}


Circle::Circle(double radius, std::vector<double> center):m_radius(radius)
{
  if(center.size() != 2)
    {
      Message::Warning("The center of the disk must be a 2D vector! Abording...");
      Message::Finalize(EXIT_FAILURE);
    }
  m_center.resize(2);
  for (int i = 0; i < 2; i ++)
    m_center[i] = center[i];
}

void Circle::GetCenter(std::vector<double> *center)
{
  center->resize(2);
  (*center)[0]=m_center[0];
  (*center)[1]=m_center[1];
}


//Equal to true if the two circles intersect, false otherwise
bool Circle::Intersect(Circle *c)
{
  double radius = c->GetRadius();
  std::vector<double> center(2);
  c->GetCenter(&center);
  double d = (m_center[0]-center[0])*(m_center[0]-center[0]) + (m_center[1]-center[1])*(m_center[1]-center[1]);
  return (d < (radius + m_radius)*(radius + m_radius) );
}

//Print stat of the circle
void Circle::Print()
{
  Message::Info("Circle of radius %g, and center (%g,%g)\n", m_radius, m_center[0], m_center[1]);
  return;
}
