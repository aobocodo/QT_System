#include "widget.h"
#include "appinit.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    Widget w;
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    w.move((wd-w.width())/2,(ht-w.height())/2);
    AppInit::Instance()->start();
    w.show();

    return a.exec();
}
