#include "plane.h"

Plane::Plane(QObject *parent, QPointF _startPoint, QVector<QPointF> &_route) :
    QObject(parent), QGraphicsItem(), route(_route)
{
    setPos(_startPoint);
    currentPoint = route.front();
    nextPoint = route[route.indexOf(currentPoint)+1];
    angle = QLineF(pos(),nextPoint).angle();
    //    angle = qRadiansToDegrees(qAcos(QLineF(currentPoint,QPointF(nextPoint.x(),currentPoint.y())).length()/QLineF(currentPoint,nextPoint).length()));
    setRotation(angle);
    qDebug() << "Route is " << route;

}

void Plane::move()
{
    if(nextPoint == route.back()) {
        currentPoint = route.back();

    }
    if (QLineF(this->pos(),nextPoint).length() > 0.5) {
        angle = QLineF(pos(),nextPoint).angle();
//        if (rotation() != 360-angle)
//            if (rotation()+angle < 360)
//                setRotation(rotation()+1);
//            else
//                setRotation(360-angle);
//        else {
            setRotation(360-angle);
            setPos(mapToParent(1,0));
//        }
        //qDebug() << angle;
        //        qDebug() << "Distance = " << QLineF(this->pos(),nextPoint).length();
    }
    else {
        qDebug() <<"====================================\n" <<route << "\n===========================================\nCurrent Point was " << currentPoint << "\nNext point was " << nextPoint;
        currentPoint = nextPoint;
        nextPoint = route[route.indexOf(currentPoint)+1];
        qDebug() << "Now next point is " << nextPoint;
        //        angle = qRadiansToDegrees(qAcos(QLineF(currentPoint,QPointF(nextPoint.x(),currentPoint.y())).length()/QLineF(currentPoint,nextPoint).length()));

        //        setRotation(angle);
    }
}


//////////////////////////////
QRectF Plane::boundingRect() const
{
    return QRectF( -50, -50, 100, 100);
}

void Plane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::red));
    QPolygonF polygon;
    polygon << QPointF(-3,-3) << QPointF(3,0) << QPointF(-3,3);
    painter->drawPolygon(polygon);
    painter->setBrush(Qt::blue);
    painter->setOpacity(0.4);
    polygon.clear();
    qreal radius = 50;
    polygon << QPointF(-(3+radius/3),-(4+radius)) << QPointF(3+radius/3,-(4+radius)) << QPointF(-(3+radius/3),4+radius) << QPointF(3+radius/3,4+radius);
    painter->drawPolygon(polygon);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

