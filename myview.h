#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QWheelEvent>

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = nullptr);
    ~MyView();

protected:
    void resizeEvent(QResizeEvent *event);

private:

};

#endif // MYVIEW_H
