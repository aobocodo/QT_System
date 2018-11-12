#ifndef BSTHREE_MAINWINDOW_H
#define BSTHREE_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class bsthree_mainwindow;
}

class bsthree_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit bsthree_mainwindow(QWidget *parent = 0);
    ~bsthree_mainwindow();
signals:
    void close_bsthree();

private:
    Ui::bsthree_mainwindow *ui;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // BSTHREE_MAINWINDOW_H
