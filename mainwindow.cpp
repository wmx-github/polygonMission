#include "mainwindow.h"
#include "wayPointLine.h"
#include "polygonField.h"
#include "calculateGeometry.h"
#include <QDebug>
#include <QGeoCoordinate>
#include <QList>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    polygonField * workField = new polygonField(this);
    workField->setCorner(QGeoCoordinate(22.76080511, 114.27268677000001));
    workField->setCorner(QGeoCoordinate(22.760423759999998, 114.27272872));
    workField->setCorner(QGeoCoordinate(22.760492840000001, 114.27306436000001));
    workField->setCorner(QGeoCoordinate(22.760642069999999, 114.27319323));
    workField->setCorner(QGeoCoordinate(22.760799590000001, 114.27332509));
    workField->setCorner(QGeoCoordinate(22.760901830000002, 114.27327414));
    workField->setSideLines();

    calculateGeometry *calculate = new calculateGeometry(this);
    calculate->setOriginPoint(workField->corners().at(0));
    //多边形面积
    qDebug()<<calculate->calculatePolygonFieldArea(workField->cornerXYList());

//    foreach(QPair<double, double>  point, workField->cornerXYList()){
//        qDebug()<< point.first << " " << point.second;
//    }
    qDebug() <<workField->corners().size();
    qDebug()<<workField->cornerXYList().size();
    qDebug() <<workField->sidelines().size();

    #define XYOut(a) qDebug()<<QString::number(a.first, 'f', 16) << QString::number(a.second, 'f', 16)
    for (int i = 0; i < workField->cornerXYList().size(); i++){
      //多边形顶点的相对坐标
      XYOut(workField->cornerXYList().at(i));
      qDebug()<<"xy2latlog"<< calculate->xy2latlog(workField->cornerXYList().at(i), calculate->originPoint());
    }

    qDebug()<<endl;

    foreach(wayPointLine *line, workField->sidelines()){
        qDebug()<<line->slope()<<line->intercept();
    }

    qDebug() << endl;
    //设置作业区
    calculate->setPolygonField(*workField);
    //多边形 参考边（被通过多边形各顶点的垂线垂直）
    wayPointLine *baseSideLine;
    //相对参考边的 最远点
    QPair<double, double> farthestPoint;
    //最远距离
    double farthestDistance;
    //垂足
    QPair<double, double> vertialPoint;
    //喷幅 暂时设置为15米
    double W = 4.0;
    //N 等分
    int N;

    baseSideLine = workField->sidelines().at(0);
    // qDebug() << baseSideLine->slope();

    farthestPoint = calculate->farthestPointToLine(*baseSideLine);
    qDebug() <<farthestPoint;

    vertialPoint = calculate->calculateVertialPoint(farthestPoint, *baseSideLine);
    qDebug() << vertialPoint;

    farthestDistance = calculate->calculatePointLineDistance(farthestPoint, *baseSideLine);
    qDebug() << farthestDistance;

    N = farthestDistance/W ;
    qDebug() << N;

    double x_unit, y_unit;
    x_unit = (farthestPoint.first - vertialPoint.first)/(2*N);
    y_unit = (farthestPoint.second - vertialPoint.second)/(2*N);
    qDebug() << x_unit<<" "<<y_unit;
    QPair<double, double>unitXY(x_unit, y_unit);
    //垂线（最远点和垂足之间的连线OP）和航线的垂足（N个点）
    //1000条航线，2000个点
    QPair<double, double> points_F_V[1000], previousPoint;
    //points_F_V[0] 为空（可以设置为垂足）
    int k ;
    for(int i = 1; i <= N; i++){
        if(i == 1){
            k = 1;
            previousPoint = vertialPoint;
        } else {
            k = 2;
            previousPoint = points_F_V[i - 1];
        }
        points_F_V[i].first = previousPoint.first + unitXY.first*k;
        points_F_V[i].second = previousPoint.second + unitXY.second*k;
        qDebug() << points_F_V[i];
    }

    qDebug() <<endl;
    qDebug() <<QStringLiteral("求航点missionPoints :");
    //求航点missionPointsGeo[j] 2000个
    QPair<double, double> tempPoint;
    QGeoCoordinate missionPointsGeo[2000];

    //直线的范围是多边形区域的顶点rage[i]
//    QPair<double, double> rage[workField->cornerXYList().size()];
//    for(int i= 0;  i < workField->sidelines().size(); i++){
//        workField->cornerXYList().at(i);
//    }

     int countOfPonts = 0;
    for(int i = 1; i <= N; i++){
        //构造航线(斜率为baseSideLine的斜率、过点PointS_F_V[i])
        wayPointLine *missionLine = new wayPointLine(baseSideLine->slope(), points_F_V[i]);
        //每条航线分别与多边形相交(在边范围内的交点）

        for(int j = 0; j < workField->sidelines().size(); j++){
            wayPointLine *l = new wayPointLine(workField->sidelines().at(j)->slope(),
                                              workField->sidelines().at(j)->intercept());
            if(j < workField->cornerXYList().size() - 1){
                tempPoint = calculate->calculateIntersectPoint(*missionLine, *l,
                                                                  workField->cornerXYList().at(j),
                                                                  workField->cornerXYList().at(j + 1));
            } else {
                tempPoint = calculate->calculateIntersectPoint(*missionLine, *l,
                                                                  workField->cornerXYList().at(j),
                                                                  workField->cornerXYList().at(0));
            }

            if(tempPoint == QPair<double, double>(INFINITY, INFINITY)){
                continue;
            } else {
                //missionPoints[j] = tempPoint;
                countOfPonts++;
            }
            qDebug() << tempPoint;
            //航点转化为经纬度
            missionPointsGeo[countOfPonts] = calculate->xy2latlog(tempPoint, calculate->originPoint());
            qDebug() << missionPointsGeo[countOfPonts];
        }
    }

    //对航点排序
    QGeoCoordinate tempGeo;
    for (int i = 0; i <countOfPonts; i++){
        if((i -1)%4 == 0&& i > 0){
            qDebug() <<i;
            tempGeo = missionPointsGeo[i];
            missionPointsGeo[i] = missionPointsGeo[i + 1];
            missionPointsGeo[i + 1] = tempGeo;
        }
    }
    //生成mission文件

    QString itemStr = {"{""\n"\
                       "\"" "id" "\"" ":%1 ,""\n"\
                       "\"" "参数1" "\"" ": 0,""\n"\
                       "\"" "参数2" "\"" ": 0,""\n"\
                       "\"" "参数3" "\"" ": 0,""\n"\
                       "\"" "参数4" "\"" ": 0,""\n"\
                       "\"" "命令" "\"" ": 16,""\n"\
                       "\"" "坐标" "\""" : [""\n"\
                           "%2,""\n"\
                           "%3,""\n"\
                           " 25""\n"\
                       "],""\n"\
                      "\""  "类型" "\"" ":"  "\"" "任务项" "\",""\n"\
                       "\"" "结构" "\"" " : 3,""\n"\
                       "\"" "自动继续" "\""" : true""\n"\
                        "},""\n" };

    QString fileStart = {"{""\n"\
                            "   \"MAV_AUTOPILOT\": 3,\n"\
                             "  \"complexItems\": [\n"\
                            "   ],\n"\
                             "  \"groundStation\": \"QGroundControl\",\n"
                            "   \"items\": [\n"};

    QString itemStrStart = {"       {""\n"};
    QString itemStrID = {"          \"" "id" "\"" ":%1 ,""\n"};
    QString itemStrParam1 = { "         \"" +QStringLiteral("参数1")+ "\"" ": 0,""\n"};
    QString itemStrParam2 = { "         \"" +QStringLiteral("参数2")+ "\"" ": 0,""\n"};
    QString itemStrParam3 = { "         \"" +QStringLiteral("参数3")+  "\"" ": 0,""\n"};
    QString itemStrParam4 = { "         \"" +QStringLiteral("参数4")+  "\"" ": 0,""\n"};
    QString itemStrCmd = {"          \"" +QStringLiteral("命令")+ "\": %1,\n"};
    QString itemStrCoordinateStart = {"         \"" +QStringLiteral("坐标")+  "\""" : [""\n"};
    QString itemStrCoordinateEnd = { "          ],""\n"};
    QString itemStrLatitude = {"                %1"};
    QString itemStrlongitude = {"                %1"};

    QString itemStrEnd = {"         \""  +QStringLiteral("类型")+  "\"" ":"  "\"" +QStringLiteral("任务项")+  "\",""\n"\
                          "         \"" +QStringLiteral("结构")+  "\"" " : 3,""\n"\
                          "         \"" +QStringLiteral("自动继续")+  "\""" : true""\n"\
                           "        }""\n" };
    QString fileEnd = {   "     ],\n"\
                          "     \"plannedHomePosition\": {\n"\
                             "          \"id\": 0,\n"\
                              "         \""+QStringLiteral("参数1")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数2")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数3")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数4")+"\": 0,\n"\
                              "         \""+QStringLiteral("命令")+"\": 16,\n"\
                              "         \""+QStringLiteral("坐标")+"\": [\n"\
                              "             22.760833000000002,\n"\
                              "             114.273055,\n"\
                              "             0\n"\
                              "         ],\n"\
                              "        \""+QStringLiteral("类型")+"\": \""+QStringLiteral("任务项")+"\",\n"\
                              "        \""+QStringLiteral("结构")+"\": 0,\n"\
                              "        \""+QStringLiteral("自动继续")+"\": true\n"\
                          "     },\n"\
                          "  \"version\": \"1.0\"\n"\
                          " }\n"
                        };

    //起飞点
    missionPointsGeo[0] = missionPointsGeo[1];
    qDebug()<<"missionPointsGeo[0]"<<missionPointsGeo[0];
    //返航点
    missionPointsGeo[countOfPonts + 1] = missionPointsGeo[countOfPonts];
///////////////////////////////////////////

    QFile itemFile("C:polygonMissionFile.mission");
    if(!itemFile.open(QIODevice::WriteOnly)){
        qDebug()<<"crop_dustingMission.cpp 114 fail open itemFile";
        return;
    }

    itemFile.write(fileStart.toUtf8());
    for(int i = 1; i <= countOfPonts + 2 ; i++){
        itemFile.write(itemStrStart.toUtf8());
        itemFile.write(itemStrID.arg(i).toUtf8());
        itemFile.write(itemStrParam1.toUtf8());

        itemFile.write(itemStrParam2.toUtf8());
        itemFile.write(itemStrParam3.toUtf8());
        itemFile.write(itemStrParam4.toUtf8());
        //插入的起飞点missionPointsGeo[0]序号为1
        if(i == 1){
            itemFile.write(itemStrCmd.arg(22).toUtf8());
        } else {
            itemFile.write(itemStrCmd.arg(16).toUtf8());
            if(i == countOfPonts+2){
                itemFile.write(itemStrCmd.arg(20).toUtf8());
            }
        }

        itemFile.write(itemStrCoordinateStart.toUtf8());
        itemFile.write(itemStrLatitude.arg(QString::number(missionPointsGeo[i -1].latitude(), 'f' , 16)).toUtf8());
        itemFile.write(",\n");
        itemFile.write(itemStrlongitude.arg(QString::number(missionPointsGeo[i -1].longitude(), 'f' , 16)).toUtf8());
        itemFile.write(",\n");
        itemFile.write("                15\n");
        itemFile.write(itemStrCoordinateEnd.toUtf8());
        itemFile.write(itemStrEnd.toUtf8());
        if(i <= countOfPonts + 1){
          itemFile.write(",");
        }
    }
    itemFile.write(fileEnd.toUtf8());

}

MainWindow::~MainWindow()
{

}

void MainWindow::generateMissionFile()
{

}
