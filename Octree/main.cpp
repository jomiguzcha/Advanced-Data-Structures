#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
#include "point.h"
#include <chrono>
#include <iostream>

void Read(QString Filename,QVector<float> &vector){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile:: Text)){
        qDebug() << "no pude";
        return;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mText.remove('v');


    QStringList strList = mText.split(QRegExp("[ ]"),QString::SkipEmptyParts);

    float aum=0.5;

    for(int i=0; i<strList.length(); i++)
    {
        vector.push_back(strList[i].toFloat()*aum);
    }

    mFile.close();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVector<float> dat;

    QString mFilename = "C:/Users/joseg/Downloads/EDA/OcTree-master/puntos3d/rex.txt";

    Read(mFilename,dat);
    qDebug() <<"puntos: " << dat.size()/3;

    double x_min = dat[0], x_max = dat[0], y_min = dat[1],
    y_max = dat[1], z_min = dat[2], z_max = dat[2];

    for (int i = 3; i < dat.size()-3; i += 3){
        dat[i] < x_min ? x_min = dat[i] : (dat[i] > x_max? x_max = dat[i]: x_max = x_max);
        dat[i+1] < y_min ? y_min = dat[i+1] : (dat[i+1] > y_max ? y_max = dat[i+1] : y_max = y_max);
        dat[i+2] < z_min ? z_min = dat[i+2] : (dat[i+2] > z_max ? z_max = dat[i+2] : z_max = z_max);
    }

    MainWindow w(Point(x_min,y_min,z_min),Point(x_max,y_max,z_max));

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<dat.size()-3; i=i+3)
    {
        w.qTree->insert(Point(dat[i],(dat[i+1]-0.1),dat[i+2]));
        w.pointsW.push_back(Point(dat[i],(dat[i+1]-0.1),dat[i+2]));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;

    qDebug() << diff.count() << " s\n";

    w.paintGL();

    w.setTitle("OpenGL QT");
    w.resize(680,680);
    w.show();

    return a.exec();
}
