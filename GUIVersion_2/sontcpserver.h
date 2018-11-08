#ifndef SONTCPSERVER_H
#define SONTCPSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QHostAddress>
#include <QMap>
#include "socketthread.h"
#include "widget.h"


class Widget;

class SonTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SonTcpServer(QObject *parent = 0);
    void StartServer();
    SocketThread *thread;
    QList<int> socketList;
    QMap<qintptr, QString> socketMap;
    int countNum;
signals:
    void emit_displayDisc(qintptr id);
    void emit_displayConn(qintptr id);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    Widget *widget;
private slots:
    void deleteTcp(qintptr id);
};

#endif // SONTCPSERVER_H
