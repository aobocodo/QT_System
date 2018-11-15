#include "log_mainwindow.h"
#include "ui_log_mainwindow.h"

log_mainwindow::log_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::log_mainwindow)
{
    ui->setupUi(this);
}

log_mainwindow::~log_mainwindow()
{
    delete ui;
}

void log_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_log();
}
