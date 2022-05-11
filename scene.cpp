#include "scene.h"

Scene::Scene() :
    QGraphicsScene()
{
    setSceneRect(0,0,100,100);
    plane = nullptr;
    rectItem = nullptr;
    lineItem = nullptr;
    lineItemPrev = nullptr;
    lineItemNext = nullptr;
    textPos = "";
}

Scene::~Scene()
{
    points.clear();
    Zones.clear();
    for(int i = 0; i < ellipses.size();i++)
        this->removeItem(ellipses[i]);
    ellipses.clear();
    for(int i = 0; i < polygons.size();i++)
        this->removeItem(polygons[i]);
    polygons.clear();
}
void Scene::addZone()
{
    msg = "\tAdding Zone";
    mouseEnable = true;
    changingZone = false;
    settingAirport = false;
}

void Scene::changeZone()
{
    msg = "\tChanging Zone";
    mouseEnable = true;
    changingZone = true;
    settingAirport = false;
}

void Scene::deleteZone()
{
    msg = "\tDeleting Zone";
    mouseEnable = true;
    changingZone = false;
    deletingZone = true;
    settingAirport = false;
}

void Scene::createAirport()
{
    msg = "\tSetting up Airport";
    mouseEnable = true;
    changingZone = false;
    deletingZone = false;
    settingAirport = true;
}

void Scene::clearAll()
{
    msg = "";
    mouseEnable = false;
    changingZone = false;
    deletingZone = false;
    settingAirport = false;
    Zones.clear();
    lines.clear();
    points.clear();
    ellipses.clear();
    polygons.clear();
    path.clear();
    while (items().size() != 0)
        removeItem(static_cast<QGraphicsItem*>(items().back()));

    //    for(int i = 0; i < polygons.size();i++)
    //        if (polygons[i] == itemAt(event->scenePos(),QTransform())){
    //            indexToChangeZone = i;
    //            removeItem(polygons[indexToChangeZone]);
    //            Zones.erase(Zones.begin()+indexToChangeZone);
    //            polygons.erase(polygons.begin() +indexToChangeZone);

    //        }

}
void Scene::completeZone()
{
    points.clear();
    points.push_back(rectItem->rect().topLeft());
    points.push_back(rectItem->rect().topRight());
    points.push_back(rectItem->rect().bottomRight());
    points.push_back(rectItem->rect().bottomLeft());
    if (!zoneChanged)
        Zones.push_back(points);
    else {
        Zones[indexToChangeZone].swap(points);

    }
    for(int i = 0; i < ellipses.size();i++)
        this->removeItem(ellipses[i]);
    ellipses.clear();
    points.clear(); /////////////////////
    for(int i = 0; i < lines.size(); i++)
        this->removeItem(lines[i]);
    lines.clear();
    removeItem(lineItem);

    removeItem(rectItem);

    if(!zoneChanged) {
        QGraphicsPolygonItem *polygon = new QGraphicsPolygonItem(QPolygonF(Zones.back()));
        polygon->setOpacity(0.5);
        polygon->setBrush(QBrush(Qt::yellow));
        polygon->setPen(Qt::NoPen);
        polygons.push_back(polygon);
        this->addItem(polygon);
    } else {
        removeItem(lineItemPrev);
        removeItem(lineItemNext);
        removeItem(polygons[indexToChangeZone]);
        polygons.removeAt(indexToChangeZone);
        QGraphicsPolygonItem *polygon = new QGraphicsPolygonItem(QPolygonF(Zones[indexToChangeZone]));
        polygon->setOpacity(0.5);
        polygon->setBrush(QBrush(Qt::yellow));
        polygon->setPen(Qt::NoPen);
        polygons.insert(indexToChangeZone,polygon);
        this->addItem(polygon);
    }
    zoneCompleted = true;
    zoneChanged = false;
    pointsCreated = false;
}


void Scene::computeWaypoints()
{
    if (Zones.isEmpty())
        return;

    QLineF _line, minLine, maxLine;
    path.push_back(airportPoint);
    /*
    int zoneIndex = 0;
    _line = QLineF(path.back(), Zones[0][0]);
    minLine = _line;
    maxLine = _line;

    for (int i = 0; i < Zones.size(); i++) {
        for(int j = 0; j < Zones[i].size(); j++) {
            _line = QLineF(path.back(), Zones[i][j]);
            if (_line.length() < minLine.length()) {
                minLine = _line;
                zoneIndex = i;
                //                polygons[i]->

            }
            //                        if (_line.length() > maxLine.length())
            //                            maxLine = _line;
        }
    }
    path.push_back(minLine.p2());
    qDebug() << path;
    indexes.push_back(zoneIndex);
    int nextZoneIndex = -1;
    if (Zones.size() > 1) {
        for (int i = 0; i < Zones.size()-1; i++) {
            qDebug() << "Cycle continues...";
            minLine.setLength(100000.0);
            for (int j = 0; j < Zones[zoneIndex].size(); j++) {
                for (int n = 0; n < Zones.size(); n++)
                    for(int k = 0; k < Zones[n].size(); k++)
                        if (!indexes.contains(n))
                            if (!path.contains(Zones[n][k])){
                                _line = QLineF(Zones[zoneIndex][j], Zones[n][k]);
                                if (_line.length() < minLine.length()) {
                                    minLine = _line;
                                    nextZoneIndex = n;
                                }
                            }
            }

            if (!path.contains(minLine.p1()))
                path.push_back(minLine.p1());
            //createWaypointsInZone(zoneIndex, path[path.size()-2], path.back());
            if (!path.contains(minLine.p2()))
                path.push_back(minLine.p2());
            //...
            //...
            //...
            //waypoints.back()

            qDebug () << "Current Zone was " << zoneIndex;
            qDebug () << "Next Zone is " << nextZoneIndex;

            zoneIndex = nextZoneIndex;
            indexes.push_back(zoneIndex);
            qDebug() << "Now path looks like \n" << path;
        }

        //        path.pop_back();
        minLine.setLength(100000.0);
        for (int i = 0; i < Zones[zoneIndex].size(); i++) {
            _line = QLineF(Zones[zoneIndex][i], path[0]);
            if(_line.length() <minLine.length())
                minLine = _line;
        }
        if (!path.contains(minLine.p1()))
            path.push_back(_line.p1());
        path.push_back(_line.p2()); //airportPoint
        // createWaypointsInZone(zoneIndex, path.back(),path.back());
    } else
        //createWaypointsInZone(zoneIndex, path.back(),path.back());


        */
    QPointF enterPoint;
    QVector<QVector<QPointF> > squares;
    QVector<QPointF> enterPoints;
    QVector<int> indexes;
    qreal radius=20; //plane scan radius;
    for (int i = 0; i < Zones.size(); i++) {
        for (int j = 0; j < Zones[i].size(); j++) {
            qDebug() << "Enterpoint #" << j << " = "<<Zones[i][j];
            if(j == Zones[i].size()-1) {
                qreal len = QLineF(Zones[i][j],Zones[i][0]).length();
                int k =1;
                do {
//                    (int k = 1; k < (int)len/radius; k++)
                    int numberOfPoints = (int)(len/radius+1);
                    if (numberOfPoints < 2)
                        numberOfPoints = 2;
                    if (Zones[i][j].rx() == Zones[i][0].rx()) {
                        if (Zones[i][j].ry() < Zones[i][0].ry())
                            enterPoint = QPointF(Zones[i][j].rx(),Zones[i][j].ry()+k*len/numberOfPoints);
                        else
                            enterPoint = QPointF(Zones[i][j].rx(),Zones[i][0].ry()+k*len/numberOfPoints);
                    } if (Zones[i][j].ry() == Zones[i][0].ry()) {
                        if (Zones[i][j].rx() < Zones[i][0].rx())
                            enterPoint = QPointF(Zones[i][j].rx()+k*len/numberOfPoints,Zones[i][j].ry());
                        else
                            enterPoint = QPointF(Zones[i][0].rx()+k*len/numberOfPoints,Zones[i][j].ry());
                    }
                    addEllipse(QRectF(enterPoint+QPointF(-0.5,-0.5),enterPoint+QPointF(0.5,0.5)));
                    enterPoints.push_back(enterPoint);
                    k++;
                } while(k<(int)len/radius);
            }
            else {
                qreal len = QLineF(Zones[i][j],Zones[i][j+1]).length();
                int k = 1;
                do {
//                    (int k = 1; k < (int)len/radius; k++)
                    int numberOfPoints = (int)(len/radius+1);
                    if (numberOfPoints < 2)
                        numberOfPoints = 2;
                    if (Zones[i][j].rx() == Zones[i][j+1].rx()) {
                        if (Zones[i][j].ry() < Zones[i][j+1].ry())
                            enterPoint = QPointF(Zones[i][j].rx(),Zones[i][j].ry()+k*len/numberOfPoints);
                        else
                            enterPoint = QPointF(Zones[i][j].rx(),Zones[i][j+1].ry()+k*len/numberOfPoints);
                    } if (Zones[i][j].ry() == Zones[i][j+1].ry()) {
                        if (Zones[i][j].rx() < Zones[i][j+1].rx())
                            enterPoint = QPointF(Zones[i][j].rx()+k*len/numberOfPoints,Zones[i][j].ry());
                        else
                            enterPoint = QPointF(Zones[i][j+1].rx()+k*len/numberOfPoints,Zones[i][j].ry());
                    }
                    addEllipse(QRectF(enterPoint+QPointF(-0.5,-0.5),enterPoint+QPointF(0.5,0.5))); ///////////////////////////////////////
                    enterPoints.push_back(enterPoint);
                    k++;
                }while(k <(int)len/radius);
            }
        }
        qDebug() << "\nEnterPoints\n" << enterPoints << "\n\n\n\n";
        squares.push_back(enterPoints);
        enterPoints.clear();
    }








    //_line = QLineF(path.back(), squares[0][0]);
    //minLine = _line;
    _line = QLineF(path.back(), squares[0][0]);
    minLine = _line;

    int indexI = -1, indexJ = -1;
    for (int k = 0; k < squares.size();k++) {
        minLine.setLength(10000);
        for (int i = 0; i < squares.size(); i++) {

            qDebug() << "NEXT";
            for (int j = 0; j < squares[i].size(); j++) {
                if (indexes.contains(i))
                    break;


                _line = QLineF(path.back(),squares[i][j]);
                //                qDebug() << "_Line = " << _line.length();
                //     qDebug() << "minLine = " << minLine.length();
                if (_line.length() <= minLine.length()) {
                    qDebug() << "True";
                    minLine = _line;
                    indexI = i;
                    indexJ = j;

                }
            }
        }
        indexes.push_back(indexI);
        path.push_back(squares[indexI][indexJ]);
        //offset;
        path.push_back(squares[indexI][(indexJ+squares[indexI].size()/2)%squares[indexI].size()]);
        qDebug() << "Added line with length = " << minLine.length() ;
    }


    path.push_back(airportPoint);
    qDebug() << path;



    qreal length = 0.0;
    for (int i = 0; i < path.size()-1; i++) {
        QPointF point1 = path[i],
                point2 = path[i+1];
        _line = QLineF(point1,point2);
        QGraphicsLineItem *linePath = new QGraphicsLineItem(_line);
        linePath->setPen(QPen(Qt::darkGreen,0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        length += _line.length();
        addItem(linePath);
    }
    qDebug() << "Path length = " << length;

}

void Scene::createWaypointsInZone(int zoneIndex, QPointF entrancePoint, QPointF exitPoint)
{
//    Q_UNUSED(entrancePoint)
//    Q_UNUSED(exitPoint)
//    QVector<QGraphicsEllipseItem *> circles;
//    qreal ellipseRadius = 1.0;
//    for (int k = 0; k < Zones.size(); k++) {
//        qreal X, Y, maxX= -1, maxY = -1, minX = 100000, minY = 100000;
//        for(int j = 0; j < Zones[k].size(); j++) {
//            if (Zones[k][j].rx() > maxX)
//                maxX = Zones[k][j].rx();
//            if (Zones[k][j].rx() < minX)
//                minX = Zones[k][j].rx();
//            if (Zones[k][j].ry() > maxY)
//                maxY = Zones[k][j].ry();
//            if (Zones[k][j].ry() < minY)
//                minY = Zones[k][j].ry();

//        }
//        QLineF line = QLineF(QPointF(minX,minY), QPointF(maxX, maxY));
//        qDebug() << line;
//        QGraphicsRectItem * rect = new QGraphicsRectItem(QRectF(QPointF(minX,minY),QPointF(maxX,maxY)));
//        rect->setBrush(Qt::NoBrush);
//        //        addItem(rect);
//        QGraphicsEllipseItem *ellipse ;
//        int iter = 0;
//        int freq = 8;
//        for (qreal i = rect->rect().topLeft().rx();
//             i < rect->rect().bottomRight().rx();
//             i +=rect->rect().width() / (sqrt(3) * ellipseRadius) / freq) {
//            for (qreal j = /*(iter%2 == 0) ? */rect->rect().topLeft().ry()/* : rect->rect().topLeft().ry() + rect->rect().height() / (3 * ellipseRadius/2)*/;
//                 j <= rect->rect().bottomRight().ry();
//                 j += rect->rect().height() / (sqrt(3) * ellipseRadius) / freq) {
//                ellipse = new QGraphicsEllipseItem(QRectF(QPointF(i-ellipseRadius,j-ellipseRadius),QPointF(i+ellipseRadius,j+ellipseRadius)));
//                circles.push_back(ellipse);
//                ellipse->setOpacity(0.1);
//                addItem(ellipse);

//            }
//            iter++;
//        }
//    }
//    // distance between X = sqrt(3) * radius
//    // distance between Y = 3 * radius / 2
//    // number of circles =  X * Y / pow(radius,2) * 2 / 3 * sqrt(3)



//    QList<QGraphicsItem *> list = polygons[zoneIndex]->collidingItems();
//    foreach (QGraphicsItem * it, list){
//        QGraphicsEllipseItem*  item = static_cast<QGraphicsEllipseItem*>(it);
//        if (item)
//            item->setOpacity(0.80);

//    }
//    //    polygons[zoneIndex]->setOpacity(0);
//    foreach (QGraphicsEllipseItem* ell, circles)
//        if (ell->opacity() == 0.1)
//            removeItem(ell);



}

void Scene::startSimFlight()
{
    if (path.isEmpty())
        return;
    plane = new Plane(this, airportPoint, path);
    addItem(plane);
    //    connect(plane, &Plane::stop, this, &Scene::stopSimFlightSlot);
}

void Scene::moveSimFlight()
{
    if (plane != nullptr)
        plane->move();

}

void Scene::addPort(QPointF pos)
{
    this->addEllipse(QRectF(QPointF(pos.x()-radius, pos.y()-radius), QPointF(pos.x()+radius, pos.y()+radius)));
    airportPoint = pos;
}

void Scene::createPoint(QPointF point)
{
    if (points.contains(point))
        return;
    points.push_back(point);
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(QRectF(QPointF(point.x()-radius, point.y()-radius), QPointF(point.x()+radius, point.y()+radius)));
    ellipse->setPen(QPen(Qt::black, 0.5));
    ellipse->setBrush(QBrush(Qt::yellow));
    ellipses.push_back(ellipse);
    addItem(ellipse);
    qDebug() << point;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (!mouseEnable)
        return;

    if (event->button() == Qt::RightButton) {
        for(int i = 0; i < ellipses.size();i++)
            this->removeItem(ellipses[i]);
        ellipses.clear();
        points.clear();
        for(int i = 0; i < lines.size(); i++)
            this->removeItem(lines[i]);
        lines.clear();
        removeItem(lineItem);
        removeItem(rectItem);
        if (changingZone) {
            if (!polygons.isEmpty()) {
                polygons[indexToChangeZone]->setBrush(Qt::yellow);
                removeItem(lineItemNext);
                removeItem(lineItemPrev);
            }
            if(dragging)
                dragging=false;
        }
        mouseEnable = false;
        creatingLine = false;
        changingZone = false;
        pointsCreated = false;
        deletingZone = false;
        msg = "";
        return;
    }
    if (deletingZone) {
        for(int i = 0; i < polygons.size();i++)
            if (polygons[i] == itemAt(event->scenePos(),QTransform())){
                indexToChangeZone = i;
                removeItem(polygons[indexToChangeZone]);
                Zones.erase(Zones.begin()+indexToChangeZone);
                polygons.erase(polygons.begin() +indexToChangeZone);

            }
        return;
    }
    if (changingZone) {

        if(!pointsCreated)
            for(int i = 0; i < polygons.size();i++)
                if (polygons[i] == static_cast<QGraphicsPolygonItem*>(itemAt(event->scenePos(),QTransform()))) {
                    polygons[i]->setBrush(QBrush(Qt::red));
                    for (int j = 0; j < Zones[i].size(); j++)
                        createPoint(Zones[i][j]);
                    indexToChangeZone = i;
                    pointsCreated = true;
                    return;
                }
        if(!dragging)
            for(int i = 0; i < ellipses.size(); i++)
                if (ellipses[i] == static_cast <QGraphicsEllipseItem*>(itemAt(event->scenePos(),QTransform()))) {
                    dragging = true;
                    indexForDragEllipse = i;
                    ellipses[indexForDragEllipse]->setBrush(QBrush(Qt::green));
                    return;
                }
        return;
    }

    if (settingAirport) {
        if (!airportExists) {
            addPort(event->scenePos());
            airportExists = true;
            settingAirport = false;
            msg = "";
        }
        mouseEnable = false;
        return;
    }
    if (event->button() != Qt::LeftButton)
        return;
    if (zoneCompleted)
        zoneCompleted = false;
    QPointF point = event->scenePos();
    createPoint(point);
    if (creatingLine) {
        completeZone();
        creatingLine = false;
    }
    //    if (QLineF(newPoint, point).length() < radius*2) {
    //        point = newPoint;
    //    } else {
    //        if (creatingLine){
    //            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(points[points.size()-2], point));
    //            lines.push_back(line);
    //            line->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    //            addItem(line);
    //            creatingLine = false;
    //        }
    //    }

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    textPos = "X = " + QString::number(event->scenePos().x()) + " Y = " + QString::number(event->scenePos().y()) + msg;
    emit mousePosChanged();
    if(!mouseEnable)
        return;
    if(changingZone) {
        if(!dragging)
            return;
        if(lineItemPrev != nullptr)
            this->removeItem(lineItemPrev);
        if(lineItemNext != nullptr)
            this->removeItem(lineItemNext);
        points[indexForDragEllipse] = event->scenePos();
        ellipses[indexForDragEllipse]->setPos(ellipses[indexForDragEllipse]->mapToParent(-event->lastScenePos()+event->scenePos()));

        QLineF linePrev, lineNext;
        if (indexForDragEllipse == 0)
            linePrev = QLineF(points.back(), points[indexForDragEllipse]);
        else
            linePrev = QLineF(points[indexForDragEllipse-1], points[indexForDragEllipse]);
        if (indexForDragEllipse == ellipses.size()-1)
            lineNext = QLineF(points.front(), points[indexForDragEllipse]);
        else
            lineNext = QLineF(points[indexForDragEllipse+1], points[indexForDragEllipse]);
        lineItemPrev = new QGraphicsLineItem(linePrev);
        lineItemPrev->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
        addItem(lineItemPrev);
        lineItemNext = new QGraphicsLineItem(lineNext);
        lineItemNext->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
        addItem(lineItemNext);
    }
    if (settingAirport)
        return;
    if(zoneCompleted)
        return;
    if(points.size() == 0)
        return;

    if(rectItem != nullptr)
        this->removeItem(rectItem);

    if(lineItem != nullptr)
        this->removeItem(lineItem);


    QLineF line = QLineF(points.back(),event->scenePos());

    if (QLineF(event->scenePos(), points.front()).length() < radius*2) {
        line = QLineF(points.back(), points.front());
        newPoint = points.front();
        rectItem = new QGraphicsRectItem(QRectF(points.back(),points.front()));
        rectItem->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
        lineItem = new QGraphicsLineItem(line);
        lineItem->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    } else {
        rectItem = new QGraphicsRectItem(QRectF(points.back(),event->scenePos()));
        rectItem->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap,Qt::RoundJoin));
        lineItem = new QGraphicsLineItem(line);
        lineItem->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

    }
    this->addItem(rectItem);
    this->addItem(lineItem);
    creatingLine = true;
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!mouseEnable)
        return;
    if(!changingZone)
        return;
    if(dragging) {
        dragging = false;
        zoneChanged = true;
        completeZone();
    }
    Q_UNUSED(event)
}


