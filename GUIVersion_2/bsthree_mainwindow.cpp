#include "bsthree_mainwindow.h"
#include "ui_bsthree_mainwindow.h"

bsthree_mainwindow::bsthree_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bsthree_mainwindow)
{
    ui->setupUi(this);
    ui->pushButton->setProperty("btn","white");
}

bsthree_mainwindow::~bsthree_mainwindow()
{
    delete ui;
}

void bsthree_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_bsthree();
}
