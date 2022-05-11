#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTimer>
#include <QDebug>
#include <qdebug.h>
#include <QtQml>

#include <QtLocation/QGeoServiceProvider>

//#include <QtLocation/QGeoServiceProvider>
//#include <QtLocation/qgeoserviceprovider.h>
#include "scene.h"
#include "planemenu.h"
#include "graphicsviewzoom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addGeoService();

private slots:
    void openFile();
    void saveFile();
    void addZone();
    void changeZone();
    void deleteZone();
    void setAirport();
    void clearScreen();
    void openPlaneMenu();
    void closePlaneMenu();
    void compute();
    void statusBarShowPos();
    void startSimFlight();
private:
    Ui::MainWindow *ui;
    Scene *scene;
    PlaneMenu *planeMenu;
    GraphicsViewZoom *zoom;
    QTimer *timer;


};

#endif // MAINWINDOW_H
