#include "bsone_mainwindow.h"
#include "ui_bsone_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


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

void bsone_mainwindow::on_pushButton_clicked()
{
    /*可以选择多个文件的方法，只需要能选一个
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/nano/shell", "All files (*.*);;Text File(*.txt)"
                );
    */
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/nano/shell", "Text File(*.txt)"
                );
    ui->lineEdit->setText(filename);
}

void bsone_mainwindow::on_bsone_btn_clicked()
{
    QStringList list;
    QString line;
    qDebug()<<"file name is "<<ui->lineEdit->text();
    QFile file(ui->lineEdit->text());
    if(!file.open(QIODevice::ReadOnly) || ui->lineEdit->text()==NULL)
        QMessageBox::information(0, "error", file.errorString());
    QTextStream in(&file);
    while(!in.atEnd()){
        line = in.readLine();
        list.append(line);
    }
    /*todo 下发给基站一*/
    qDebug()<<"list is "<<list;
}
