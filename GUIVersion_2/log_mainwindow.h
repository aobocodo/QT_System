#ifndef LOG_MAINWINDOW_H
#define LOG_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class log_mainwindow;
}

class log_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit log_mainwindow(QWidget *parent = 0);
    ~log_mainwindow();
signals:
    void close_log();

private:
    Ui::log_mainwindow *ui;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // LOG_MAINWINDOW_H
