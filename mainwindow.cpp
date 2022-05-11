#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    planeMenu = new PlaneMenu();
    zoom = new GraphicsViewZoom(ui->myView);
    //    zoom->setModifiers(Qt::ControlModifier);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveFile);

    connect(ui->actionAddNewZone, &QAction::triggered, this, &MainWindow::addZone);
    connect(ui->actionChangePlaneSettings, &QAction::triggered, this, &MainWindow::openPlaneMenu);
    connect(ui->actionCompute, &QAction::triggered, this, &MainWindow::compute);
    connect(ui->actionChangeExistingZone, &QAction::triggered, this, &MainWindow::changeZone);
    connect(ui->actionDeleteZone, &QAction::triggered, this, &MainWindow::deleteZone);
    connect(ui->actionSet_Airport, &QAction::triggered, this, &MainWindow::setAirport);
    connect(ui->actionClear_Screen, &QAction::triggered, this, &MainWindow::clearScreen);
    connect(planeMenu, &PlaneMenu::closeMenu, this, &MainWindow::closePlaneMenu);


    scene = new Scene();
    ui->myView->setScene(scene);
    connect(scene, &Scene::mousePosChanged, this, &MainWindow::statusBarShowPos);

    connect(ui->actionStartSimFlight, &QAction::triggered, this, &MainWindow::startSimFlight);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{

}

void MainWindow::saveFile()
{

}

void MainWindow::addZone()
{
    scene->addZone();

}

void MainWindow::changeZone()
{
    scene->changeZone();
}

void MainWindow::deleteZone()
{
    scene->deleteZone();
}

void MainWindow::setAirport()
{
    scene->createAirport();
}

void MainWindow::clearScreen()
{
    scene->clearAll();
}

void MainWindow::openPlaneMenu()
{

    planeMenu->show();
}

void MainWindow::closePlaneMenu()
{
    planeMenu->close();
}

void MainWindow::compute()
{
    scene->computeWaypoints();
}

void MainWindow::statusBarShowPos()
{
    ui->statusBar->showMessage(scene->textPos);

}

void MainWindow::startSimFlight()
{
    scene->startSimFlight();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, scene, &Scene::moveSimFlight);
    timer->start(50);


}

