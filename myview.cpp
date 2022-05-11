#include "myview.h"
#include <qmath.h>
#include <QDebug>
MyView::MyView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setMouseTracking(true);
}

MyView::~MyView()
{

}

void MyView::resizeEvent(QResizeEvent *event)
{
    fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatioByExpanding);
    QGraphicsView::resizeEvent(event);
}
