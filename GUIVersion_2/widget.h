#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include "config_mainwindow.h"
#include "optimize_mainwindow.h"
#include "heal_mainwindow.h"
#include "bsone_mainwindow.h"
#include "bstwo_mainwindow.h"
#include "bsthree_mainwindow.h"
#include "log_mainwindow.h"
#include "sontcpserver.h"
#include "qcustomplot.h"

class SonTcpServer;
class config_mainwindow;
class optimize_mainwindow;
class heal_mainwindow;
class bsone_mainwindow;
class bstwo_mainwindow;
class bsthree_mainwindow;
class log_mainwindow;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QMenu *menu_one,*menu_two,*menu_three;
    config_mainwindow *cg_frame;
    optimize_mainwindow *opt_frame;
    heal_mainwindow *heal_frame;
    bsone_mainwindow *bsone_frame;
    bstwo_mainwindow *bstwo_frame;
    bsthree_mainwindow *bsthree_frame;
    log_mainwindow *log_frame;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::Widget *ui;
//    zyx新增
    SonTcpServer* server;
    qintptr current_id;
    void initForm();
    QString show_data();
signals:
    void emit_socketData(QString , qintptr);
private slots:
    void menu_one_trigged(QAction* action);
    void cg_close();
    void opt_close();
    void heal_close();
    void menu_two_trigged(QAction* action);
    void bsone_close();
    void bstwo_close();
    void bsthree_close();
    void menu_three_trigged(QAction* action);
    void log_close();
    void on_btn_max_clicked();
    void on_btn_min_clicked();
    void on_btn_close_clicked();
    void transferData(QString data, qintptr id);
    void to_main(QString data);
    void displayDisconnected(qintptr id);
    void displayConnected(qintptr id);
};

#endif // WIDGET_H
