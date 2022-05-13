#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtLocation/private/qdeclarativegeomapitembase_p.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setWindowTitle("Aerophotography");

//  quickView = new QQuickWidget;
//  quickView->setParent(this);
//  ui->quickWidget->setSource(QUrl("qrc:/map.qml"));
  ui->quickWidget->hide();

//  if(QQuickItem *map = ui->quickWidget->findChild<QQuickItem*>("map1")){
//      QQmlComponent component(ui->quickWidget->engine(), QUrl("qrc:/rect.qml"));
//      if(QDeclarativeGeoMapItemBase *rect = qobject_cast<QDeclarativeGeoMapItemBase*>(component.create(ui->quickWidget->rootContext()))){
//          bool status = QMetaObject::invokeMethod(map,
//                                    "addMapItem",
//                                    Qt::DirectConnection,
//                                    Q_ARG(QDeclarativeGeoMapItemBase*, rect));
//          Q_ASSERT(status);
//      }
//  }
  scene = new Scene();
  ui->myView->setScene(scene);

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

//  ui->myView->setParent(ui->quickWidget);
  connect(scene, &Scene::mousePosChanged, this, &MainWindow::statusBarShowPos);
  connect(ui->actionStartSimFlight, &QAction::triggered, this, &MainWindow::startSimFlight);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::setSceneBackgroundImage);
  connect(scene, &Scene::cursorChangedToDrag, this, &MainWindow::cursorDrag);
  connect(scene, &Scene::cursorStandart, this, &MainWindow::cursorStandart);

//  scene->setParent(ui->quickWidget);



  //    QQmlApplicationEngine engine;
  //    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  //    quickView->show();
  //    scene->setBackgroundBrush(quickView->grabFramebuffer());

  //    scene->setBackgroundBrush(QBrush(Qt::cyan));
  addGeoService();
  scene->addPort(QPointF(100,250));
  scene->setScale(20);
  //    timer->start(5);
  //    scene->setBackgroundBrush(QBrush(QPixmap("map.png")));

}


void MainWindow::cursorDrag()
{
    this->setCursor(scene->cursor);
}

void MainWindow::cursorStandart()
{
  this->setCursor(scene->cursor);
}

void MainWindow::setSceneBackgroundImage()
{
  QString filename;
  QString path = QFileDialog::getOpenFileName(this, "Открыть файл", filename, "*.png *.jpg" );
  if(path != "")
      scene->setBackgroundImage(path);
     qDebug() << "Path = " << path ;
     qDebug() << "Filename = " << filename;

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::addGeoService()
{
//  QStringList services =  QGeoServiceProvider::availableServiceProviders();
//   if (!services.isEmpty()) {
//       qDebug() << services;
//    }

  //    QGraphicsGeoMap
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
  scene->computePath();
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

