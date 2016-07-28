#ifndef polygonField_H
#define polygonField_H

#include <QObject>
#include <QGeoCoordinate>
#include <QList>
#include <QPair>
#include "wayPointLine.h"

//多边形
class polygonField : public QObject
{
    Q_OBJECT
public:
    explicit polygonField(QObject *parent = 0);
    ~polygonField();

    void setCorner(QGeoCoordinate &cornerPoint);
    void setSideLines(void);
    void setArea(double area);
    QList <wayPointLine *> sidelines(void);
    QList <QGeoCoordinate >  corners(void);
    QList < QPair<double, double> > cornerXYList(void);
    double area(void);
    //double sideLength(void);
signals:

public slots:
private:
    QList <QGeoCoordinate > m_cornerList;
    QList < QPair<double, double> > m_cornerXYList;
    QList <wayPointLine *> m_sideLineList;
    double m_fieldArea;
    //重载运算符 =
    polygonField& operator =(const polygonField &field);
};

#endif // polygonField_H
