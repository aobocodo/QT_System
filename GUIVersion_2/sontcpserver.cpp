#include "sontcpserver.h"

SonTcpServer::SonTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    widget = dynamic_cast<Widget*>(parent);
}
void SonTcpServer::StartServer()
{
    if(!this->listen(QHostAddress::Any, 12345))
    {
        qDebug() << "Coule not start server";
    }
    else
    {
        qDebug() << "listening.....";
    }
    countNum = 0;

}
void SonTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<< socketDescriptor <<"connecting....";
    /*按顺序添加socket描述符，先连先进*/
     socketList.append(socketDescriptor);
     /*字典用于连接显示值与描述符
    socketDescriptor为key,eNB序号为value*/
     countNum++;
     //QString str = QString("eNB%1").arg(countNum);
     /*固定描述符与eNB*/
     QString str;
     switch (socketDescriptor) {
     case 21:
         str = QString("eNB1");
         socketMap[socketDescriptor] = str;
         break;
     case 23:
         str = QString("eNB2");
         socketMap[socketDescriptor] = str;
         break;
     case 26:
         str = QString("eNB3");
         socketMap[socketDescriptor] = str;
         break;
     default:
         break;
     }
     //socketMap[socketDescriptor] = str;
     /*发送信号，用于在屏幕显示连接*/
     emit emit_displayConn(socketDescriptor);
     /*查看list及map情况*/
     qDebug() << "countNum:" << countNum;

/*启动socket线程
 */
     thread = new SocketThread(socketDescriptor, this);
//     发送描述符信号

    // connect(widget->cg_frame, SIGNAL(emit_confeNb(QStringList)), thread, SLOT(sendTest(QStringList))); 失败,因为创建connect时cg_frame还没创建
     connect(widget, SIGNAL(emit_socketData(QString,qintptr)), thread, SLOT(sendTest(QString, qintptr)));//姑且这样吧,感觉效率很低
     /*从列表及字典中删除断开的连接*/
     connect(thread, SIGNAL(disconnectTcp(qintptr)), this, SLOT(deleteTcp(qintptr)));
     connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
     thread->start();
}
void SonTcpServer::deleteTcp(qintptr id)
{
    emit emit_displayDisc(id);
    this->socketList.removeAll(id);
    this->socketMap.remove(id);
}
