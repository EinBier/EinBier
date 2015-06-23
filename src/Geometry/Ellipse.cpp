#include <Geometry/Geometry.h>
#include <Geometry/Circle.h>
#include <Common/Message.h>

#include <stdlib.h>
#include <math.h>

Ellipse::Ellipse(double xradius, double yradius, double x, double y)
    : p_xradius(xradius), p_yradius(yradius)
{
    p_center.resize(2);
    p_center[0]=(x);
    p_center[1]=(y);
}


Ellipse::Ellipse(double xradius, double yradius, std::vector<double> center)
    : p_xradius(xradius), p_yradius(yradius)
{
    if(center.size() != 2)
    {
        Message::Error("The center of the disk must be a 2D vector! Abording...");
        Message::Finalize(EXIT_FAILURE);
    }
    p_center.resize(2);
    p_center[0] = center[0];
    p_center[1] = center[1];
}

void Ellipse::setParam(double xradius, double yradius, double x, double y)
{
    p_xradius =xradius;
    p_yradius =yradius;
    p_center.resize(2);
    p_center[0]=(x);
    p_center[1]=(y);

}


void Ellipse::GetCenter(std::vector<double> *center)
{
    center->resize(2);
    (*center)[0]=p_center[0];
    (*center)[1]=p_center[1];
}


//Equal to true if the two ellipses intersect, false otherwise
bool Ellipse::Intersect(Ellipse *e)
{
    double xradius = e->GetXRadius();
    double yradius = e->GetYRadius();
    std::vector<double> center(2);
    e->GetCenter(&center);
    if(center[0] == p_center[0] && center[1] == p_center[1])
        return true;
  //Distance between centers
    double dO1O2 = (p_center[0]-center[0])*(p_center[0]-center[0])
        + (p_center[1]-center[1])*(p_center[1]-center[1]);
    dO1O2 = sqrt(dO1O2);
  //cosTheta
    double cosTheta = (xradius-p_xradius)/dO1O2;
    double sinTheta = (yradius-p_yradius)/dO1O2;
    //Point on the line O1-O2 AND on the current ellipse
    double xA = p_center[0] + p_xradius*cosTheta;
    double yA = p_center[1] + p_yradius*sinTheta;
    //Same for other ellipse
    double xB = center[0] + xradius*(-cosTheta);
    double yB = center[1] + yradius*(-sinTheta);
    //Distance between A and B (squared)
    double dAB = (xB-xA)*(xB-xA) + (yB-yA)*(yB-yA);
    dAB = sqrt(dAB);
    //If dAB <= dO1O2 then they intersect
    return (dO1O2-dAB <= 0);
}

//Print stat of the Ellipse
void Ellipse::Print()
{
    Message::Info("Ellipse of x-radius %g, y-radius %g, and center (%g,%g)\n",
                  p_xradius, p_yradius, p_center[0], p_center[1]);
    return;
}
