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

Plane::~Plane() {
  delete pixmap;
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
//        qDebug() <<"====================================\n" <<route << "\n===========================================\nCurrent Point was " << currentPoint << "\nNext point was " << nextPoint;
        currentPoint = nextPoint;
        nextPoint = route[route.indexOf(currentPoint)+1];
//        qDebug() << "Now next point is " << nextPoint;
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
    QPolygonF polygon;
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::blue);
    painter->setOpacity(0.4);    
    qreal radius = 69;
//    painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
//    painter->drawEllipse(QPointF(0,-radius/2),radius/3,radius/2);
//    painter->drawEllipse(QPointF(0,radius/2),radius/3,radius/2);
    qreal h = radius*sqrt(3)/2;
    polygon << QPointF(-(h),-(radius)) << QPointF(h,-(radius)) << QPointF(-(h),radius) << QPointF(h,radius);
    painter->drawPolygon(polygon);
    polygon.clear();
    painter->setOpacity(1);
    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::red));
//    polygon << QPointF(-3,-3) << QPointF(4,0) << QPointF(-3,3);
//    painter->drawPolygon(polygon);
    pixmap = new QPixmap("../images/airplane.png");
    QTransform transf = painter->transform();
    transf.scale(0.25,0.25);
    transf.rotate(90);
    painter->setTransform(transf);
    painter->drawPixmap(boundingRect(),*pixmap,pixmap->rect());
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

