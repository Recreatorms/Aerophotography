#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QVector>
#include <qmath.h>
#include <QDebug>
#include <QCursor>
#include <QUrl>
#include "plane.h"
#define PI 3.1415926
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene();
    ~Scene() override;
    void addZone();
    void changeZone();
    void deleteZone();
    void createAirport();

    void clearAll();

    void completeZone();
    void computePath();
    void createWaypointsInZone();
    void addPort(QPointF pos);
    void createPoint(QPointF point);
    QString textPos;
    void startSimFlight();
    void setScale(int _scale ) { scale = _scale;}
    void setBackgroundImage(QString filename);

    QCursor cursor;
public slots:
    void moveSimFlight();
signals:
    void cursorChangedToDrag();
    void cursorStandart();
    void mousePosChanged();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
    bool mouseEnable = false,
         creatingLine = false,
         changingZone = false,
         deletingZone = false,
         settingAirport = false,
         zoneCompleted = false;
    bool zoneChanged = false,
         pointsCreated = false,
         dragging = false,
         computed = false;

    int indexToChangeZone,
        indexForDragEllipse;

    QPointF airportPoint;
    QPointF  lastPos,
             newPos;
    bool draggingScene;
    QVector<QPointF> path;
    QVector<QPointF> points;
    QVector<QVector<QVector<QPointF> > > squares;
    QVector<QVector<QPointF> > Zones;
    QVector<QGraphicsLineItem *> lines;
    QVector<QGraphicsEllipseItem *> ellipses;
    QVector<QGraphicsEllipseItem *> enterPointEllipses;
    QVector<QGraphicsLineItem *> linesPath;
    QVector<QPointF> route;
    QVector<QGraphicsPolygonItem *> polygons;
    QPointF newPoint;
    QGraphicsRectItem *rectItem;
    QGraphicsLineItem *lineItem,
                      *lineItemPrev,
                      *lineItemNext;
    QString msg;
//    qreal radius = 2*scale;
    QGraphicsPathItem *pathItem;
    QGraphicsPixmapItem *airport;
    Plane *plane;
    qreal planeRadius = 1000;
    int scale;
    QPixmap backgroundPixmap;
    QString filename = "../images/Map1.png";
};

#endif // SCENE_H
