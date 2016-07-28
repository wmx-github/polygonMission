#ifndef WAYPOINTLINE_H
#define WAYPOINTLINE_H

#include <QObject>
#include <QPair>

//直线类
class wayPointLine : public QObject
{
    Q_OBJECT
public:
        //wayPointLine(QObject *parent = 0);

        //2点式
        wayPointLine(QPair<double, double> a, QPair<double, double> b);
        //点斜式
        wayPointLine(double slope, QPair<double, double> a);
        //斜截式
        wayPointLine(double slope, double intercept);
        ~wayPointLine();

        //
        double slope(){return m_slope;}
        double intercept(){return m_intercept;}

//        QPair<double, double> originPoint(void);
//        //把坐标转化为以m_originPoint为原点的相对坐标
//        QPair<double, double> relativeCoordinate(QPair<double, double> point);
private:
        //斜率
        double m_slope;
        //截距
        double m_intercept;
};

#endif // WAYPOINTLINE_H

