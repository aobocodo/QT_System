#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString macAddress;
    /*获取mac地址*/
    macAddress = getHostMacAddress();
    ui->lineEdit->setText(macAddress);
    /*初始化socket*/
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 12345);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    //no edit
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //插入行，item居中
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QTableWidgetItem *element = new QTableWidgetItem("ue1");
    element->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,element);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem("ue2"));

    //根据ue id删除行
    QList<QTableWidgetItem *> itemList;
    for(int i =0; i<ui->tableWidget->rowCount();i++){
        itemList<<ui->tableWidget->item(i,0);
    }
    foreach(QTableWidgetItem *item, itemList){
        if(item->text()=="ue2"){
            ui->tableWidget->removeRow(item->row());
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connected()
{
    qDebug() << "连接上了";
    ui->eNB_one_output->append(show_data()+"已连接到服务器!");
}
void MainWindow::readyRead()
{
    QString recvStr,confStr;
    QByteArray recvByte;
    //qint8 transP, recvP;
    //double centerF, bandWidth;
    QString transP, recvP, centerF, bandWidth;
    recvByte = socket->readAll();
    recvStr = recvByte;
    qDebug() << recvStr;
    //切割字符串，取出配置部分，并用逗号替换空格，并移除字符串两端的空白符
    confStr = recvStr.section("              ", 2, 6).replace("              ", ",").trimmed();
    qDebug() << confStr;
    centerF = confStr.section(",",0,0);
    bandWidth = confStr.section(",",1,1);
    transP = confStr.section(",",2,2);
    recvP = confStr.section(",",3,3);
    ui->lineEdit_3->setText(bandWidth);
    ui->lineEdit_5->setText(transP);
    ui->lineEdit_4->setText(centerF);
    ui->lineEdit_9->setText(recvP);
    ui->eNB_one_output->append(show_data()+"配置成功！");
}


//Mac地址获取函数
QString MainWindow::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces(); //获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for (int i=0; i<nCnt; i++)
    {
        //qDebug() << nets[i].hardwareAddress();
        //如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Ｍac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

QString MainWindow::show_data()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    QString data = "["+str+"]";
    return data;
}
