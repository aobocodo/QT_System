#include "bstwo_mainwindow.h"
#include "ui_bstwo_mainwindow.h"

bstwo_mainwindow::bstwo_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bstwo_mainwindow)
{
    ui->setupUi(this);
    ui->pushButton->setProperty("btn","white");
}

bstwo_mainwindow::~bstwo_mainwindow()
{
    delete ui;
}

void bstwo_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_bstwo();
}
