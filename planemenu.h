#ifndef PLANEMENU_H
#define PLANEMENU_H

#include <QWidget>

namespace Ui {
    class PlaneMenu;
}

class PlaneMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PlaneMenu(QWidget *parent = nullptr);
    ~PlaneMenu();
signals:
    void closeMenu();
private:
    Ui::PlaneMenu *ui;
};

#endif // PLANEMENU_H
