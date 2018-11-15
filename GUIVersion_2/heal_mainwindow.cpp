#include "heal_mainwindow.h"
#include "ui_heal_mainwindow.h"

heal_mainwindow::heal_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::heal_mainwindow)
{
    ui->setupUi(this);
    ui->heal_btn->setProperty("btn","white");
}

heal_mainwindow::~heal_mainwindow()
{
    delete ui;
}

void heal_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_heal();
}
