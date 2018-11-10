#ifndef CONFIG_MAINWINDOW_H
#define CONFIG_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "sontcpserver.h"

//zyx:新增用于传递server参数
class  SonTcpServer;

namespace Ui {
class config_mainwindow;
}

class config_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit config_mainwindow(SonTcpServer *server, QWidget *parent = 0);
    ~config_mainwindow();
    bool check_on(QString check); //用来判断on和off按钮
    void print_to_main();
signals:
    void close_cg();

    void emit_confeNb(QString eNbconf, qintptr ); //传递eNB配置给socket下发
    void emit_to_main(QString info);
private:
    Ui::config_mainwindow *ui;
    SonTcpServer * server1;
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_clicked();

    void on_cg_btn_cover_clicked();
    void on_cg_btn_power_clicked();
    void on_cg_btn_output_clicked();
};

#endif // CONFIG_MAINWINDOW_H
