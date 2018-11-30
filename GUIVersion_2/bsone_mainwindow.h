#ifndef BSONE_MAINWINDOW_H
#define BSONE_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class bsone_mainwindow;
}

class bsone_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit bsone_mainwindow(QWidget *parent = 0);
    ~bsone_mainwindow();
signals:
    void close_bsone();

private:
    Ui::bsone_mainwindow *ui;
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_clicked();
    void on_bsone_btn_clicked();
};

#endif // BSONE_MAINWINDOW_H
