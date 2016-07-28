#include "calculateGeometry.h"
#include <QDebug>

calculateGeometry::calculateGeometry(QObject *parent) : QObject(parent)
{

}

calculateGeometry::~calculateGeometry()
{

}

//判断是否为同一直线
bool calculateGeometry::isTheSameLine(wayPointLine l1, wayPointLine l2)
{
    if(l1.slope()!=l2.slope()||l1.intercept()!=l2.intercept()){
        return false;
    } else {
        return true;
    }
}

void calculateGeometry::setOriginPoint(const QGeoCoordinate &originPoint)
{
    m_originPoint = originPoint;
}

QGeoCoordinate calculateGeometry::originPoint(void)
{
    return m_originPoint;
}

void calculateGeometry::setPolygonField(polygonField &field)
{
   //利用polygonField 运算符重载 =
    m_workField = &field;
}

QPair<double, double> calculateGeometry::farthestPointToLine(wayPointLine &sideLine)
{
    //其中sideLine可以取多边形的各边m_workField->sidelines().at(i)
    int j = 0;
    double farthest = 0.0f;
    double tempDistance;
    for(int i = 0; i < m_workField->cornerXYList().size(); i++){
        tempDistance = calculatePointLineDistance(m_workField->cornerXYList().at(i), sideLine);
        if(tempDistance > farthest){
            farthest = tempDistance;
            j = i;
        } else {
           //
        }
    }
    //返回最远点坐标
    return m_workField->cornerXYList().at(j);
}

//void calculateGeometry::setW(double spraySpan)
//{
//    m_W = spraySpan;
//}

//经纬度转xy
QPair<double, double> calculateGeometry::latlog2xy(QGeoCoordinate A, QGeoCoordinate B)
{

    double x, y;
    //x和y的单位是m,lat和lon的单位是°
     x = (double)(A.latitude() - B.latitude()) * LATLON_TO_M;            //
     double lon_to_m = cosf(B.latitude()* DEG_TO_RAD) * LATLON_TO_M;    //
     y = (double)(A.longitude() - B.longitude()) * lon_to_m;
     QPair<double, double> pair(x, y);
     return pair;
}

//xy转经纬度
QGeoCoordinate calculateGeometry::xy2latlog(QPair<double, double> point, QGeoCoordinate originPoint)
{
    QGeoCoordinate temp;
    temp.setLatitude(point.first/LATLON_TO_M +originPoint.latitude());

    double lon_to_m = cosf(originPoint.latitude()* DEG_TO_RAD) * LATLON_TO_M;
    temp.setLongitude(point.second/lon_to_m + originPoint.longitude());

    return temp;
}

//距离，经纬度
double calculateGeometry::latlog2xyDistance(QGeoCoordinate A, QGeoCoordinate B)
{
      double x, y;
//    //x和y的单位是m,lat和lon的单位是°
//     x = (double)(A.latitude() - B.latitude()) * LATLON_TO_M;            //
//     double lon_to_m = cosf(B.latitude()* DEG_TO_RAD) * LATLON_TO_M;    //
//     y = (double)(A.longitude() - B.longitude()) * lon_to_m;

    QPair<double, double> pair = latlog2xy(A, B);
    x = pair.first;
    y = pair.second;

     //返回2点间的距离

     return qSqrt(x*x+y*y);

}

//距离，xy
double calculateGeometry::xyDistance(QPair<double, double> aPoint, QPair<double, double> bPoint)
{
    double x, y;
    x = aPoint.first - bPoint.first;
    y = aPoint.second - bPoint.second;
    return qSqrt(x*x+y*y);
}

//点到直线的距离
double calculateGeometry::calculatePointLineDistance(QPair<double, double> p, wayPointLine &l)
{
    return xyDistance(p, calculateVertialPoint(p, l));
}

//计算垂足坐标
QPair<double, double> calculateGeometry::calculateVertialPoint(QPair<double, double> point, wayPointLine &line)
{
    //(x1, y1)为垂足（point ——> line作垂线）
    double x0, y0, x1, y1;
    x0 = point.first;
    y0 = point.second;

    x1 = (x0/line.slope() + y0 - line.intercept())/(line.slope() + 1/line.slope());
    y1 = line.slope() * x1 + line.intercept();

    //QPair<double, double> a(x0, y0);
    QPair<double, double> b(x1, y1);
    return b;
}

//2平行线的距离
double calculateGeometry::calculateLinelineDistance(wayPointLine l1, wayPointLine l2)
{
    return(qFabs(l1.intercept() - l2.intercept())/qSqrt(1 + l1.slope() * l1.slope()));
}

//2直线的交点
QPair<double, double> calculateGeometry::calculateIntersectPoint(wayPointLine &l1, wayPointLine &l2,  QPair<double, double> ragePoint1, QPair<double, double> ragePoint2)
{
    //设2直线交点（x0, y0)
    double x0, y0;
    x0 = (l1.intercept() - l2.intercept())/(l2.slope() -l1.slope());
    y0 = l1.slope() * x0 + l1.intercept();
    QPair<double, double> temp(x0, y0);
    if(ragePoint1.first <= temp.first && temp.first <= ragePoint2.first ||
            ragePoint2.first <= temp.first && temp.first <= ragePoint1.first){
        return temp;
    } else {
        //如果不在范围内，则返回无穷值
        return  QPair<double, double>(INFINITY, INFINITY);
    }
    //return temp;
}

//计算多边形面积
double calculateGeometry::calculatePolygonFieldArea(QList< QPair<double, double> > pointList)
{
    double s = 0.0f;
    int n = pointList.size();
    for(int i = 1; i < n-1; i++){
        s += det(pointList[0],pointList[i],pointList[i+1]);
    }
    qDebug()<< 0.5*qFabs(s);
    return 0.5*qFabs(s);
}

//向量的叉乘 [行列式]
double calculateGeometry::det(QPair<double, double> p0, QPair<double, double> p1, QPair<double, double> p2)
{
    //参考原点为p0
    return (p1.first-p0.first)*(p2.second-p0.second)-(p1.second-p0.second)*(p2.first-p0.first);
}

