#ifndef BSTWO_MAINWINDOW_H
#define BSTWO_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class bstwo_mainwindow;
}

class bstwo_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit bstwo_mainwindow(QWidget *parent = 0);
    ~bstwo_mainwindow();
signals:
    void close_bstwo();

private:
    Ui::bstwo_mainwindow *ui;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // BSTWO_MAINWINDOW_H
