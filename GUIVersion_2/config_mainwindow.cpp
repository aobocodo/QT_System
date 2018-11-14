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
    server1 = server;
    //no edit
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //let first column can not edit
    for(int i =0; i<3;i++){
        QTableWidgetItem *itab = ui->tableWidget->item(i,0);
        itab->setFlags(itab->flags() & (~Qt::ItemIsEditable));
    }
    ui->tableWidget->setColumnWidth(7, 130);
    //在tablewidget中添加comboBox
    for(int i=0; i<3;i++){
        QComboBox *combox_state = new QComboBox();
        combox_state->addItem("工作");
        combox_state->addItem("待机");
        ui->tableWidget->setCellWidget(i,1,(QWidget*)combox_state);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_point = new QComboBox();
        combox_point->addItem("2.66MHz");
        combox_point->addItem("2.36MHz");
        ui->tableWidget->setCellWidget(i,2,(QWidget*)combox_point);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_bw = new QComboBox();
        combox_bw->addItem("5.00M");
        combox_bw->addItem("10.0M");
        combox_bw->addItem("20.0M");
        ui->tableWidget->setCellWidget(i,3,(QWidget*)combox_bw);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_tx = new QComboBox();
        combox_tx->addItem("95.00dB");
        combox_tx->addItem("105.0dB");
        combox_tx->addItem("115.0dB");
        ui->tableWidget->setCellWidget(i,4,(QWidget*)combox_tx);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_rx = new QComboBox();
        combox_rx->addItem("90.00dB");
        combox_rx->addItem("100.0dB");
        combox_rx->addItem("110.0dB");
        ui->tableWidget->setCellWidget(i,5,(QWidget*)combox_rx);
    }
    for(int i=0; i<3;i++){
        QComboBox *combox_pw = new QComboBox();
        combox_pw->addItem("20dBm");
        combox_pw->addItem("30dBm");
        combox_pw->addItem("40dBm");
        ui->tableWidget->setCellWidget(i,6,(QWidget*)combox_pw);
    }
    //隔行变色(没用上)
    ui->tableWidget->setAlternatingRowColors(true);
    //ui->tableWidget->horizontalHeader()->setObjectName("hHeader");水平方向的header属性无法设置
    //设置垂直方向上的header属性
    ui->tableWidget->verticalHeader()->setObjectName("vHeader");
    ui->pushButton->setProperty("btn","white");//设置属性，与qss文件中的样式对应

}

config_mainwindow::~config_mainwindow()
{
    delete ui;
}

//窗口关闭事件
void config_mainwindow::closeEvent(QCloseEvent *)
{
    emit close_cg();
}

void config_mainwindow::on_pushButton_clicked()
{
    //发送信号给主界面
    QString head=show_data()+"<font color=red>手动配置参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}

//发送配置信息给主界面
void config_mainwindow::print_to_main()
{
    //发送信号给主界面
    QString head="ID                      状态                 频点                     带宽               发射增益             接收增益               功率                         PRB分配";
    emit emit_to_main(head);
    //遍历表中所有配置信息&发送
    for(int i=0; i<3; i++){
        QString data;
        QTableWidgetItem *itab = ui->tableWidget->item(i,0);
        QString itabtext = itab->text();
        data = itabtext+":  ";
        for(int j=1; j<7; j++){        
            QComboBox* combo=(QComboBox*)ui->tableWidget->cellWidget(i,j);
            data=data+"            "+combo->currentText()+"  ";
        }
        QTableWidgetItem *prb = ui->tableWidget->item(i,7);
        QString prbtext = prb->text();
        data= data+"            "+prbtext;
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

QString config_mainwindow::show_data()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    QString data = "["+str+"]";
    return data;
}

//判断基站状态
bool config_mainwindow::check_on(QString check)
{
    if (check.contains("工作"))
        return true;
    else
        return false;
}

//点击覆盖最优
void config_mainwindow::on_cg_btn_cover_clicked()
{
    QComboBox* combo_value=(QComboBox*)ui->tableWidget->cellWidget(1,1);
    combo_value->setCurrentIndex(combo_value->findText("待机"));
    QComboBox* combo_value_1=(QComboBox*)ui->tableWidget->cellWidget(1,3);
    combo_value_1->setCurrentIndex(combo_value_1->findText("10.0M"));
    QString head=show_data()+"<font color=red>[覆盖最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}

//点击性能最优
void config_mainwindow::on_cg_btn_power_clicked()
{
    QString head=show_data()+"<font color=red>[性能最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}

//点击吞吐量最优
void config_mainwindow::on_cg_btn_output_clicked()
{
    QString head=show_data()+"<font color=red>[吞吐量最优]策略参数：</font>";
    emit emit_to_main(head);
    print_to_main();
}
