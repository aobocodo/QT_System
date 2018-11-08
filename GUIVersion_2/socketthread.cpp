#include "socketthread.h"

SocketThread::SocketThread(qintptr id, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = id;
}
void SocketThread::run()
{
    qDebug()<< socketDescriptor <<"starting thread";
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    //qDebug() << "是否触发！！！" << i;
    qDebug() << socketDescriptor <<"Client Connected";

    exec();
}
void SocketThread::readyRead()
{

    QByteArray Data0;
    Data0 = socket->readAll();
    qDebug() << socketDescriptor <<"Data in:" <<Data0;
}
void SocketThread::disconnected()
{
    qDebug()<<socketDescriptor<<"Disconnected";
    emit disconnectTcp(socketDescriptor);
    socket->deleteLater();
    exit(0);
}
void SocketThread::sendTest(QString data, qintptr id)
{
    qDebug() << "测试成功";
    if (id == socketDescriptor)
        socket->write(data.toLocal8Bit());

}
