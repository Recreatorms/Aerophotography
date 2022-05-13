#ifndef PLANE_H
#define PLANE_H

#include <QObject>
#include <QGraphicsItem>
//#include <QPen>
#include <QPainter>
#include <qmath.h>
#include <QDebug>
class Plane : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Plane(QObject *parent, QPointF _startPoint, QVector<QPointF> &_route);
    ~Plane();
    void move();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QVector<QPointF> &route;
    QPointF currentPoint, nextPoint;
    qreal angle;
    QPixmap *pixmap;
};

#endif // PLANE_H
