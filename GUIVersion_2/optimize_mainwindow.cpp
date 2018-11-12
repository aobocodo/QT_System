#include "optimize_mainwindow.h"
#include "ui_optimize_mainwindow.h"

optimize_mainwindow::optimize_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::optimize_mainwindow)
{
    ui->setupUi(this);
}

optimize_mainwindow::~optimize_mainwindow()
{
    delete ui;
}

void optimize_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_opt();
}
