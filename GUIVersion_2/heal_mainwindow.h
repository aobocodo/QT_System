#ifndef HEAL_MAINWINDOW_H
#define HEAL_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class heal_mainwindow;
}

class heal_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit heal_mainwindow(QWidget *parent = 0);
    ~heal_mainwindow();
signals:
    void close_heal();

private:
    Ui::heal_mainwindow *ui;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // HEAL_MAINWINDOW_H
