#include "polygonField.h"
#include "calculateGeometry.h"
#include <QDebug>
//自定义输出语句, 控制输出精度、格式
#define GeoOut(a) qDebug()<<QString::number((a.latitude()), 'f' , 16)<<QString::number((a.longitude()), 'f' , 16)
#define XYOut(a) qDebug()<<QString::number(a.first, 'f', 16) << QString::number(a.second, 'f', 16)
polygonField::polygonField(QObject *parent) : QObject(parent)
{

}

polygonField::~polygonField()
{

}
    //设置多边形的顶点
void polygonField::setCorner( QGeoCoordinate &cornerPoint)
{
    m_cornerList.append(cornerPoint);

//    for(int i = 0; i < m_cornerList.size(); i++){
//        GeoOut(m_cornerList.at(i));
//    }
   // GeoOut(cornerPoint);

    //计算类
    calculateGeometry *calculate = new calculateGeometry(this);
    //多边形第一点设为坐标原点
    calculate->setOriginPoint(m_cornerList.at(0));
    //把坐标转化为相对原点的参考系
    m_cornerXYList.append(calculate->latlog2xy(cornerPoint, calculate->originPoint()));
}
    //设置多边形的边
void polygonField::setSideLines()
{
    for(int i = 0; i < m_cornerList.size(); i++){
        if(i ==(m_cornerList.size() - 1)){
            m_sideLineList.append(new wayPointLine(m_cornerXYList.at(i), m_cornerXYList.at(0)));
        } else {
            m_sideLineList.append( new wayPointLine(m_cornerXYList.at(i), m_cornerXYList.at(i + 1)));
        }  
       // qDebug()<<m_sideLineList.at(i)->slope()<<m_sideLineList.at(i)->intercept();
        //XYOut(m_cornerXYList.at(i));
    }
}

void polygonField::setArea(double area)
{
    m_fieldArea = area;
}

QList <wayPointLine *> polygonField::sidelines()
{
    return m_sideLineList;
}

QList <QGeoCoordinate > polygonField::corners()
{
    return m_cornerList;
}

QList<QPair<double, double> > polygonField::cornerXYList()
{
    return m_cornerXYList;
}

double polygonField::area()
{
    return m_fieldArea;
}

polygonField& polygonField::operator =(const polygonField &field)
{
    this->m_cornerList = field.m_cornerList;
    this->m_cornerXYList =field.m_cornerXYList;
    this->m_fieldArea = field.m_fieldArea;
    this->m_sideLineList = field.m_sideLineList;
    return *this;
}

//double polygonField::sideLength()
//{

//}

