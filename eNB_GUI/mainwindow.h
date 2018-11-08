#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QList>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString getHostMacAddress();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;

private slots:
    void connected();
    void readyRead();
};

#endif // MAINWINDOW_H
