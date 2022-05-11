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
    void computeWaypoints();
    void createWaypointsInZone(int zoneIndex, QPointF entrancePoint, QPointF exitPoint);
    void addPort(QPointF pos);
    void createPoint(QPointF point);
    QString textPos;
    void startSimFlight();
public slots:
    void moveSimFlight();

signals:

    void mousePosChanged();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    bool mouseEnable = false,
         creatingLine = false,
         changingZone = false,
         deletingZone = false,
         settingAirport = false,
         zoneCompleted = false;
    bool airportExists = false;
    bool zoneChanged = false,
         pointsCreated = false,
         dragging = false;

    int indexToChangeZone,
        indexForDragEllipse;

    QPointF airportPoint;
    QVector<QPointF> path;
    QVector<QPointF> points;
    QVector<QVector<QPointF> > Zones;
    QVector<QGraphicsLineItem *> lines;
    QVector<QGraphicsEllipseItem *> ellipses;

    QVector<QGraphicsPolygonItem *> polygons;
    QPointF newPoint;
    QGraphicsRectItem *rectItem;
    QGraphicsLineItem *lineItem,
                      *lineItemPrev,
                      *lineItemNext;
    QString msg;
    qreal radius = 2;

    Plane *plane;
};

#endif // SCENE_H
