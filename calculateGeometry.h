#ifndef CALCULATEGEOMETRY_H
#define CALCULATEGEOMETRY_H

#include <QObject>
#include <QPair>
#include <QGeoCoordinate>
#include "wayPointLine.h"
#include "polygonfield.h"
#include <QtMath>

//坐标系转换latlog2xy
// scaling factor from degrees to meters at equater
//LATLON_TO_M ==  DEG_TO_RAD * RADIUS_OF_EARTH
#define LATLON_TO_M 111319.5f
//π/180°
#define DEG_TO_RAD 0.017453292519943295769236907684886f

//几何计算类
class calculateGeometry : public QObject
{
    Q_OBJECT
public:
    explicit calculateGeometry(QObject *parent = 0);
    ~calculateGeometry();
    bool isTheSameLine(wayPointLine l1, wayPointLine l2);
    //设置坐标系原点
    void setOriginPoint(const QGeoCoordinate &originPoint);
    QGeoCoordinate originPoint(void);
    //设置待计算多边形
    void setPolygonField(polygonField &field);
    //求多边形的最远点(相对于参考边)
    QPair<double, double> farthestPointToLine(wayPointLine &sideLine);
    //void setW(double spraySpan);

    //经纬度变相对坐标
    QPair<double, double> latlog2xy(QGeoCoordinate A, QGeoCoordinate B);
    //相对坐标变经纬度
    QGeoCoordinate xy2latlog(QPair<double, double> point, QGeoCoordinate originPoint);

    //2点间的距离
    double latlog2xyDistance(QGeoCoordinate A, QGeoCoordinate B);
    double xyDistance(QPair<double, double> aPoint, QPair<double, double> bPoint);

    //点到直线的距离
    double calculatePointLineDistance(QPair<double, double> p, wayPointLine &l);
    //求垂足
    QPair<double, double> calculateVertialPoint(QPair<double, double> point, wayPointLine &line);

    //2平行线之间的距离
    double calculateLinelineDistance(wayPointLine l1, wayPointLine l2);
    //2直线的交点
    QPair<double, double> calculateIntersectPoint(wayPointLine &l1, wayPointLine &l2, QPair<double, double> ragePoint1, QPair<double, double> ragePoint2);
    //多边形的面积
    double calculatePolygonFieldArea(QList < QPair<double, double> > pointList);
    //向量的叉乘
    double det(QPair<double, double> p0, QPair<double, double> p1, QPair<double, double> p2);

signals:

public slots:
private:
    //坐标系原点
    QGeoCoordinate m_originPoint;
    //待计算多边形
    polygonField *m_workField;
    //多边形 参考边（被通过多边形各顶点的垂线垂直）
   // wayPointLine *m_baseSideLine;
    //相对参考边的 最远点
    //QPair<double, double> m_farthestPoint;
    //垂足
    //QPair<double, double> m_vertialPoint;
    //喷幅
    //double m_W;
};

#endif // CALCULATEGEOMETRY_H
