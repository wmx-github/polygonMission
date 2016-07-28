#include "wayPointLine.h"

//wayPointLine::wayPointLine(QObject *parent) : QObject(parent)
//{

//}

wayPointLine::wayPointLine(QPair<double, double> a, QPair<double, double> b)
{
    //斜率
    m_slope = (b.second- a.second)/(b.first - a.first);
    //截距
    m_intercept = b.second - m_slope*b.first;
}

wayPointLine::wayPointLine(double slope, QPair<double, double> a)
{
    m_slope = slope;
    m_intercept = a.second - m_slope*a.first;
}

wayPointLine::wayPointLine(double slope, double intercept)
{
    m_slope = slope;
    m_intercept = intercept;
}

wayPointLine::~wayPointLine()
{

}

//void wayPointLine::setOriginPoint(QPair<double, double> originPoint)
//{
//    m_originPoint = originPoint;
//}

//QPair<double, double> wayPointLine::originPoint()
//{
//    return m_originPoint;
//}

//QPair<double, double> wayPointLine::relativeCoordinate(QPair<double, double> point)
//{
//    point.first -= m_originPoint.first;
//    point.second -= m_originPoint.second;
//    return point;
//}

