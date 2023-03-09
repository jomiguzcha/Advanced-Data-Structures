#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QMouseEvent>

#include "point.h"
#include "octant.h"
#include "octree.h"

using namespace std;
class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(Point min, Point max, QWidget *parent = 0);
    ~MainWindow();

    vector<Point> pointsW;//contiene todos los puntos
    vector<Point> linesA;
    vector<Point> linesB;
    vector<Point*> pSelec;
    Octree<100,100>* qTree;
    float zoomScale;

    void drawSun();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    //void keyPressEvent(QKeyEvent *);
    //bool pointIN(Point a);

    virtual void paintGL();//se usa

    void printQT(Octant* quad);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    //void findC(OctTree **p);
    //bool circleInt(Point tl, Point br);
    //bool circleInt2(Point tl, Point br);


    //bool pointC(Point a,Point max, Point min);// in range
    //void pushPoint(OctTree **p);

    Point cCircle;
    float angle;
    bool pressLB;
    Point cSphere;
    float radio;
private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
};

#endif // MAINWINDOW_H
