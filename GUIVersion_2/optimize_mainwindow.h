#ifndef OPTIMIZE_MAINWINDOW_H
#define OPTIMIZE_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class optimize_mainwindow;
}

class optimize_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit optimize_mainwindow(QWidget *parent = 0);
    ~optimize_mainwindow();
signals:
    void close_opt();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::optimize_mainwindow *ui;
};

#endif // OPTIMIZE_MAINWINDOW_H
