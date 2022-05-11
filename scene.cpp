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


void Scene::computePath()
{
  if (Zones.isEmpty())
    return;
  QVector<int> indexes;
  int offsetDistance = 15;
  int number_of_points_on_side_1, number_of_points_on_side_2;
  QPointF entrancePoint, exitPoint;
  QLineF _line, minLine;

  // ====================================================
  // Creating points on the borders of the Zones

  createWaypointsInZone();
  // ====================================
  // Creating Path

  path.push_back(airportPoint);
  QPainterPath myPainterPath(path.back());
  for (int i = 0; i < squares.size(); i++) {
      int indexI = -1, indexJ = -1, indexK = -1, counter = 0;
      _line = QLineF(path.back(), squares[0][0][0]);
      minLine = _line;
      minLine.setLength(100000.0);


      // CYCLE FOR CREATING AN INITIAL POINT
      for (int j = 0; j < squares[i].size(); j++) { // <---------------- число сторон на области (default = 4)
          if (indexes.contains(i))
            break;
          for (int k = 0; k < squares[i][j].size();k++) { // <----------- число точек на одной грани

              _line = QLineF(path.back(),squares[i][j][k]);
              if (_line.length() < minLine.length()) {
                  minLine = _line;
                  indexI = i;
                  indexJ = j;
                  number_of_points_on_side_1 = squares[indexI][indexJ].size();
                  number_of_points_on_side_2 = squares[indexI][(indexJ+1)%squares[indexI].size()].size();
                  if (number_of_points_on_side_1 < number_of_points_on_side_2) { // if(_line.length() > maxLine.length()) {
                      // choosing indexJ side
                      // BUT
                      //                      if (QLineF(squares[indexI][indexJ][k], squares[indexI][(indexJ+1)%squares[indexI].size()][0]                               ).length() < minLine.length() ||
                      //                          QLineF(squares[indexI][indexJ][k], squares[indexI][(indexJ+1)%squares[indexI].size()][squares[indexI][indexJ].size()-1]).length() < minLine.length() ||
                      //                          QLineF(squares[indexI][indexJ][k], squares[indexI][(indexJ-1)%squares[indexI].size()][0]                               ).length() < minLine.length() ||
                      //                          QLineF(squares[indexI][indexJ][k], squares[indexI][(indexJ-1)%squares[indexI].size()][squares[indexI][indexJ].size()-1]).length() < minLine.length() )
                      //                        { // случай №5
                      //                        }
                      indexK = k;
                      entrancePoint = squares[indexI][indexJ][indexK]; // Входная точка - которая уже на искомой зоне
                      exitPoint     = squares[indexI][(indexJ+squares[indexI].size()/2)%squares[indexI].size()][indexK]; // Выходная точка

                    } else {
                      QLineF minLine2 = minLine;
                      minLine2.setLength(10000);
                      for (int k2 = 0; k2 < squares[indexI][(indexJ+1)%squares[indexI].size()].size(); k2++) {
                          _line = QLineF(path.back(),squares[indexI][indexJ][k2]);
                          if (_line.length() < minLine2.length()) {
                              minLine2 = _line;
                              indexK = k2;
                            }
                        }
                      indexJ = (indexJ+1)%squares[indexI].size();
                      entrancePoint = squares[indexI][indexJ][indexK]; // Входная точка - которая уже на искомой зоне
                      exitPoint     = squares[indexI][(indexJ+squares[indexI].size()/2)%squares[indexI].size()][indexK]; // Выходная точка
                    }
                }
            }
        }


      qDebug() << "added new waypoint with\nindexI = " << indexI << "\nindexJ = " << indexJ << "\nindexK = "<< indexK;
      indexes.push_back(indexI);

      QPointF offsetPoint = entrancePoint;


      if (entrancePoint.rx() > exitPoint.rx())
        offsetPoint.setX(offsetPoint.rx()+offsetDistance); ;
      if (entrancePoint.ry() > exitPoint.ry())
        offsetPoint.setY(offsetPoint.ry()+offsetDistance);
      if (entrancePoint.rx() < exitPoint.rx())
        offsetPoint.setX(offsetPoint.rx()-offsetDistance);
      if (entrancePoint.ry() < exitPoint.ry())
        offsetPoint.setY(offsetPoint.ry()-offsetDistance);



      if (!path.empty())
        myPainterPath.cubicTo(path.back(), offsetPoint,entrancePoint);
      else
        myPainterPath.quadTo(offsetPoint, entrancePoint);

      path.push_back(offsetPoint);
      path.push_back(entrancePoint);
      path.push_back(exitPoint);
      myPainterPath.lineTo(path.back());
      //  CYCLE FOR CLOSING A ZONE
      //  IF IT HAS MORE THAT 1 POINT ON BORDER
      if (squares[indexI][indexJ].size() > 1) {
          qDebug() << "we need another point on same border";
          bool scanComplete = false;
          bool goingRight = false;
          int initialIndexK = indexK;
          counter = 1;
          if (indexK < squares[indexI][indexJ].size()-1)
            goingRight = true;
          else
            goingRight = false;
          QPointF nextPoint, middlePoint, oppositeNextPoint;
          while (!scanComplete) {
              indexJ = (indexJ+squares[indexI].size()/2)%squares[indexI].size();
              if (goingRight) {
                  if (indexK+1 < squares[indexI][indexJ].size()) {
                      middlePoint = QPointF((squares[indexI][indexJ][indexK]+squares[indexI][indexJ][indexK+1])/2);

                      nextPoint   = QPointF((squares[indexI][indexJ][indexK+1]));
                      oppositeNextPoint = QPointF(squares[indexI][(indexJ+squares[indexI].size()/2)%squares[indexI].size()][indexK+1]);
                      if (path.back().rx() == nextPoint.rx()) {
                          if (path.back().rx() > oppositeNextPoint.rx())
                            middlePoint.setX(middlePoint.rx()+offsetDistance*2/3);
                          else
                            middlePoint.setX(middlePoint.rx()-offsetDistance*2/3);
                        }
                      if (path.back().ry() == nextPoint.ry()) {
                          if (path.back().ry() > oppositeNextPoint.ry())
                            middlePoint.setY(middlePoint.ry()+offsetDistance*2/3);
                          else
                            middlePoint.setY(middlePoint.ry()-offsetDistance*2/3);
                        }
                      myPainterPath.quadTo(middlePoint,nextPoint);

                      path.push_back(middlePoint);
                      path.push_back(nextPoint);
                      path.push_back(oppositeNextPoint);
                      myPainterPath.lineTo(path.back());
                      indexK = indexK+1;
                      counter++;
                    } else
                    scanComplete = true;
                }
              else {
                  if (indexK-1 > -1) {
                      middlePoint = QPointF((squares[indexI][indexJ][indexK]+squares[indexI][indexJ][indexK-1])/2);

                      nextPoint   = QPointF((squares[indexI][indexJ][indexK-1]));
                      oppositeNextPoint = QPointF(squares[indexI][(indexJ+squares[indexI].size()/2)%squares[indexI].size()][indexK-1]);
                      if (path.back().rx() == nextPoint.rx()) {
                          if (path.back().rx() > oppositeNextPoint.rx())
                            middlePoint.setX(middlePoint.rx()+offsetDistance*2/3);
                          else
                            middlePoint.setX(middlePoint.rx()-offsetDistance*2/3);
                        }
                      if (path.back().ry() == nextPoint.ry()) {
                          if (path.back().ry() > oppositeNextPoint.ry())
                            middlePoint.setY(middlePoint.ry()+offsetDistance*2/3);
                          else
                            middlePoint.setY(middlePoint.ry()-offsetDistance*2/3);
                        }

                      myPainterPath.quadTo(middlePoint,nextPoint);

                      path.push_back(middlePoint);
                      path.push_back(nextPoint);
                      path.push_back(oppositeNextPoint);
                      myPainterPath.lineTo(path.back());
                      indexK = indexK-1;
                      counter++;
                    } else
                    scanComplete = true;
                }

              qDebug() << "counter = " << counter << "max = " << squares[indexI][indexJ].size();
              if (scanComplete )
                if (counter != squares[indexI][indexJ].size()) {
                    scanComplete = false;
                    goingRight = !goingRight;
                    indexK = initialIndexK;
                  }
            }

          offsetPoint = path.back();
          if (nextPoint.rx() > oppositeNextPoint.rx())
            offsetPoint.setX(offsetPoint.rx()-offsetDistance); ;
          if (nextPoint.ry() > oppositeNextPoint.ry())
            offsetPoint.setY(offsetPoint.ry()-offsetDistance);
          if (nextPoint.rx() < oppositeNextPoint.rx())
            offsetPoint.setX(offsetPoint.rx()+offsetDistance);
          if (nextPoint.ry() < oppositeNextPoint.ry())
            offsetPoint.setY(offsetPoint.ry()+offsetDistance);

          path.push_back(offsetPoint);


        } else {

          offsetPoint = exitPoint;
          if (entrancePoint.rx() > exitPoint.rx())
            offsetPoint.setX(offsetPoint.rx()-offsetDistance); ;
          if (entrancePoint.ry() > exitPoint.ry())
            offsetPoint.setY(offsetPoint.ry()-offsetDistance);
          if (entrancePoint.rx() < exitPoint.rx())
            offsetPoint.setX(offsetPoint.rx()+offsetDistance);
          if (entrancePoint.ry() < exitPoint.ry())
            offsetPoint.setY(offsetPoint.ry()+offsetDistance);

          path.push_back(offsetPoint);
        }
    }
  qDebug() << "Added line with length = " << minLine.length() ;



  myPainterPath.quadTo(path.back(),airportPoint);
  path.push_back(airportPoint);

  qDebug() << path;


  // ===================================
  // Drawing Lines
  qreal length = 0.0;

  addPath(myPainterPath);

  qDebug() << "Path length = " << length;

}

void Scene::createWaypointsInZone()
{
  //    Q_UNUSED(entrancePoint)
  //    Q_UNUSED(exitPoint)
  QVector<QVector<QPointF> > border;
  QVector<QPointF> pointsOnBorder;
  QPointF enterPoint;

  qreal radius=30; //plane scan radius;
  for (int i = 0; i < Zones.size(); i++) {
      for (int j = 0; j < Zones[i].size(); j++) {
          qDebug() << "Enterpoint #" << j << " = "<<Zones[i][j];
          if(j == Zones[i].size()-1) {
              qreal len = QLineF(Zones[i][j],Zones[i][0]).length();
              int k =1;
              int numberOfPoints = (int)(len/radius+1);
              do {
                  //                    (int k = 1; k < (int)len/radius; k++)
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
                  //                  addEllipse(QRectF(enterPoint+QPointF(-0.5,-0.5),enterPoint+QPointF(0.5,0.5)));
                  pointsOnBorder.push_back(enterPoint);
                  k++;
                } while(k<=(int)len/radius);
              border.push_back(pointsOnBorder);
              pointsOnBorder.clear();
            }
          else {
              qreal len = QLineF(Zones[i][j],Zones[i][j+1]).length();
              int k = 1;
              int numberOfPoints = (int)(len/radius+1);
              do {
                  //                    (int k = 1; k < (int)len/radius; k++)
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
                  //                  addEllipse(QRectF(enterPoint+QPointF(-0.5,-0.5),enterPoint+QPointF(0.1,0.5))); ///////////////////////////////////////
                  pointsOnBorder.push_back(enterPoint);
                  k++;
                } while(k <=(int)len/radius);
              border.push_back(pointsOnBorder);
              pointsOnBorder.clear();
            }
        }
      qDebug() << "\nEnterPoints\n" << border << "\n\n\n\n";
      squares.push_back(border);
      border.clear();
    }
  qDebug() << "squares = " << squares;

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


