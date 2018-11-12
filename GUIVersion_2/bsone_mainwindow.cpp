#include "bsone_mainwindow.h"
#include "ui_bsone_mainwindow.h"

bsone_mainwindow::bsone_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bsone_mainwindow)
{
    ui->setupUi(this);
//    ui->bsone_lab1->setAlignment(Qt::AlignCenter);
//    ui->bsone_lab2->setAlignment(Qt::AlignCenter);
    ui->bsone_btn->setProperty("btn","white");
}

bsone_mainwindow::~bsone_mainwindow()
{
    delete ui;
}

void bsone_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_bsone();
}
