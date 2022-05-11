#include "graphicsviewzoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

#include <QDebug>

GraphicsViewZoom::GraphicsViewZoom(MyView *view) :
    QObject(view), _view(view)
{
    _view->viewport()->installEventFilter(this);
    _modifiers = Qt::ControlModifier;
    _zoomFactorBase = 1.0015;

}

void GraphicsViewZoom::gentleZoom(double factor)
{

//    qDebug() << _view->viewportTransform();
    if (factor < 1 && (_view->viewportTransform().m31() >= 1 || _view->viewportTransform().m32() >= 1))
    {
        QTransform transform = QTransform(_view->viewportTransform());
        transform.setMatrix(transform.m11(),transform.m12(),transform.m13(),transform.m21(),transform.m22(),transform.m23(),0,0,transform.m33());
        _view->setTransform(transform);
        return;
    }
    _view->scale(factor,factor);
    _view->centerOn(targetScenePos);
    QPointF deltaViewportPos = targetViewportPos - QPointF(_view->viewport()->width() / 2.0, _view->viewport()->height() / 2.0);
    QPointF viewportCenter = _view->mapFromScene(targetScenePos) - deltaViewportPos;
    _view->centerOn(_view->mapToScene(viewportCenter.toPoint()));


    emit zoomed();
}

void GraphicsViewZoom::setModifiers(Qt::KeyboardModifiers modifiers)
{
    _modifiers = modifiers;
}

void GraphicsViewZoom::setZoomFactorBase(double value)
{
    _zoomFactorBase = value;
}

bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPointF delta = targetViewportPos - mouseEvent->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
            targetViewportPos = mouseEvent->pos();
            targetScenePos = _view->mapToScene(mouseEvent->pos());
        }
    } else if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        if (QApplication::keyboardModifiers() == _modifiers) {
            if(wheelEvent->orientation() ==  Qt::Vertical) {
                double angle = wheelEvent->angleDelta().y();
                double factor = qPow(_zoomFactorBase, angle);
                gentleZoom(factor);
                return true;
            }
        }
    }
    Q_UNUSED(object)
    return false;
}

