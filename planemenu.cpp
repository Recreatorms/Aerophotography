#include "planemenu.h"
#include "ui_planemenu.h"

PlaneMenu::PlaneMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaneMenu)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    connect(ui->pushButton, &QPushButton::clicked, this, &PlaneMenu::closeMenu);
}

PlaneMenu::~PlaneMenu()
{
    delete ui;
}
