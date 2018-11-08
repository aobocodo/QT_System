#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"
#include <QDesktopWidget>
#include <QPushButton>
#include <QDebug>


int cg_i=0;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();


//    server初始化操作
    server = new SonTcpServer(this);
    server->StartServer();
    //用于显示eNB连接和断开
    connect(server,SIGNAL(emit_displayDisc(qintptr)), this, SLOT(displayDisconnected(qintptr)));
    connect(server,SIGNAL(emit_displayConn(qintptr)), this, SLOT(displayConnected(qintptr)));
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event){
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btn_max_clicked();//双击标题放大
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void Widget::initForm(){
    this->setProperty("form",true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    menu_one = new QMenu();
    menu_one->addAction("自配置");
    menu_one->addAction("自优化");
    menu_one->addAction("自治愈");

    connect(menu_one, SIGNAL(triggered(QAction*)),this,SLOT(menu_one_trigged(QAction*)));


    menu_two = new QMenu();
    menu_two->addAction("基站一");
    menu_two->addAction("基站二");
    menu_two->addAction("基站三");
    connect(menu_two,SIGNAL(triggered(QAction*)),this,SLOT(menu_two_trigged(QAction*)));

    IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->btn_min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btn_max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btn_close, QChar(0xf00d));
    ui->widgetTitle->installEventFilter(this);
    ui->widgetTitle->setProperty("form","title");
    ui->widgetTop->setProperty("nav","top");
    ui->labTitle->setFont(QFont("Microsoft Yahei", 20));

    QSize icoSize(32,32);


    int icoWidth=85;
    QList<QToolButton *> btns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns){
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        btn->setPopupMode(QToolButton::InstantPopup);
    }
    ui->btnBs->setIconSize(QSize(42, 42));
    ui->btnMg->setMenu(menu_one);
    ui->btnBs->setMenu(menu_two);
}

void Widget::menu_one_trigged(QAction *action)
{
    qDebug()<<"hello "<<action->text();
    QString name = action->text();
    if(name=="自配置"&&cg_i==0){
            ui->textBrowser->append(QString("<font color=black>准备配置基站</font>"));
            cg_i=1;
            QDesktopWidget *desk=QApplication::desktop();
            int wd=desk->width();
            int ht=desk->height();
            cg_frame = new config_mainwindow(server, this);
            cg_frame->setProperty("config","white");
            cg_frame->setWindowTitle("自配置");
            connect(cg_frame,SIGNAL(close_cg()),this,SLOT(cg_close()));
            cg_frame->move(((wd-600)/2),(ht-400)/2);
            cg_frame->setFixedSize(600,400);
            cg_frame->show();
            connect(cg_frame, SIGNAL(emit_confeNb(QString, qintptr)), this, SLOT(transferData(QString, qintptr)));
            connect(cg_frame,SIGNAL(emit_to_main(QString)),this,SLOT(to_main(QString)));

    }


}

void Widget::cg_close()
{
    cg_i=0;
}

void Widget::menu_two_trigged(QAction *action)
{
    action->text();
    qDebug()<<"hello "<<action->text();
}






void Widget::on_btn_max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        this->resize(wd,ht);
    }
    this->setProperty("canMove", true);
    max = !max;
}

void Widget::on_btn_min_clicked()
{
    showMinimized();
}

void Widget::on_btn_close_clicked()
{
    this->close();
}
void Widget::to_main(QString data)
{
    ui->textBrowser->append(data);
}
void Widget::transferData(QString data, qintptr id)
{
    emit emit_socketData(data, id);
}
void Widget::displayDisconnected(qintptr id)
{
    ui->textBrowser->append(QString("<font color=red>%1 断开</font>").arg(server->socketMap.value(id)));
}
void Widget::displayConnected(qintptr id)
{
    ui->textBrowser->append(QString("<font color=red>%1 连接</font>").arg(server->socketMap.value(id)));
}
