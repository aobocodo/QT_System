#include "config_mainwindow.h"
#include "ui_config_mainwindow.h"
#include <array>
#include <QComboBox>
#include <QDebug>



config_mainwindow::config_mainwindow(SonTcpServer *server, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::config_mainwindow)
{
    ui->setupUi(this);
    ui->menubar->show();
    server1 = server;
    for(int i=0; i<3;i++){
        QComboBox *combox_state = new QComboBox();
        combox_state->addItem("工作");
        combox_state->addItem("待机");
        ui->tableWidget->setCellWidget(i,1,(QWidget*)combox_state);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_point = new QComboBox();
        combox_point->addItem("2.66");
        combox_point->addItem("2.36");
        ui->tableWidget->setCellWidget(i,2,(QWidget*)combox_point);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_bw = new QComboBox();
        combox_bw->addItem("5.00");
        combox_bw->addItem("10.0");
        combox_bw->addItem("20.0");
        ui->tableWidget->setCellWidget(i,3,(QWidget*)combox_bw);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_tx = new QComboBox();
        combox_tx->addItem("95");
        combox_tx->addItem("105");
        combox_tx->addItem("115");
        ui->tableWidget->setCellWidget(i,4,(QWidget*)combox_tx);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_rx = new QComboBox();
        combox_rx->addItem("90");
        combox_rx->addItem("100");
        combox_rx->addItem("110");
        ui->tableWidget->setCellWidget(i,5,(QWidget*)combox_rx);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_pw = new QComboBox();
        combox_pw->addItem("20");
        combox_pw->addItem("30");
        combox_pw->addItem("40");
        ui->tableWidget->setCellWidget(i,6,(QWidget*)combox_pw);
    }
    ui->tableWidget->setAlternatingRowColors(true);
    //ui->tableWidget->horizontalHeader()->setObjectName("hHeader");
    ui->tableWidget->verticalHeader()->setObjectName("vHeader");
    ui->pushButton->setProperty("btn","white");

}

config_mainwindow::~config_mainwindow()
{
    delete ui;
}

void config_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_cg();
}

void config_mainwindow::on_pushButton_clicked()
{

    QString head="<font color=red>手动配置参数：</font>";
    emit emit_to_main(head);
    print_to_main();

        //结束
        /*
        for(int m=0; m<7; m++){
            qDebug()<<"item is "<<QString::fromStdString(data[m]);
        }
        */

/*   测试server是否正确传递
    if (!server1->socketList.isEmpty())
        qDebug() << server1->socketList.at(0);
        */
}

void config_mainwindow::print_to_main()
{
    QString head="ID                      状态             频点              带宽        发射增益    接收增益    功率";
    emit emit_to_main(head);
    for(int i=0; i<3; i++){
        QString data;
        QTableWidgetItem *itab = ui->tableWidget->item(i,0);
        QString itabtext = itab->text();
        data = itabtext+":  ";
        for(int j=1; j<7; j++){
            QComboBox* combo=(QComboBox*)ui->tableWidget->cellWidget(i,j);
            data=data+"            "+combo->currentText()+"  ";
        }
        emit emit_to_main(data);
        if (!check_on(data))
        {
            qDebug()<<itabtext <<":你选择了OFF,请选择ON进行传输!!!";
            continue;
        }
        for (int i=0; i<server1->socketMap.size(); i++)
        {
            if (!server1->socketList.empty())
            {
                if (server1->socketMap.value(server1->socketList.at(i)) == itabtext)
                {
                    qDebug() << "ABCDEFGHIJKLMNOGQRSTUVWXYZ";
                    emit emit_confeNb(data, server1->socketList.at(i)); //发送数据以及套接字
                    break;
                }
            }
        }

    }
}
bool config_mainwindow::check_on(QString check)
{
    if (check.contains("工作"))
        return true;
    else
        return false;
}

void config_mainwindow::on_cg_btn_cover_clicked()
{
    QComboBox* combo_value=(QComboBox*)ui->tableWidget->cellWidget(1,1);
    combo_value->setCurrentIndex(combo_value->findText("待机"));
    QComboBox* combo_value_1=(QComboBox*)ui->tableWidget->cellWidget(1,3);
    combo_value_1->setCurrentIndex(combo_value_1->findText("10.0"));
    QString head="<font color=red>[覆盖最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}

void config_mainwindow::on_cg_btn_power_clicked()
{
    QString head="<font color=red>[性能最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}



void config_mainwindow::on_cg_btn_output_clicked()
{
    QString head="<font color=red>[吞吐量最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}
