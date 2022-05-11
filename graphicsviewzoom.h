#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include "myview.h"
class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    GraphicsViewZoom(MyView *view);
    void gentleZoom(double factor);
    void setModifiers(Qt::KeyboardModifiers modifiers);
    void setZoomFactorBase(double value);
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void zoomed();
private:
    MyView *_view;
    Qt::KeyboardModifiers _modifiers;
    double _zoomFactorBase;
    QPointF targetScenePos, targetViewportPos;

};

#endif // GRAPHICSVIEWZOOM_H
