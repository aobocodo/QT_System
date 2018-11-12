#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"
#include <QDesktopWidget>
#include <QPushButton>
#include <QDebug>


int cg_i=0;//标识符，判断config界面是否打开
int opt_i=0;//标识符
int heal_i=0;//标识符
int bsone_i=0;//标识符
int bstwo_i=0;//标识符
int bsthree_i=0;//标识符


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();//画第一层界面


    //server初始化操作
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
    //双击标题放大
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btn_max_clicked();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::initForm(){
    this->setProperty("form",true);//设置属性，与qss文件中的样式对应
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);//去除系统自带的最大化，最小化，关闭按钮

    //为管理界面添加菜单
    menu_one = new QMenu();
    menu_one->addAction("自配置");
    menu_one->addAction("自优化");
    menu_one->addAction("自治愈");
    //添加点击菜单后对应的事件
    connect(menu_one, SIGNAL(triggered(QAction*)),this,SLOT(menu_one_trigged(QAction*)));

    //为基站界面添加菜单
    menu_two = new QMenu();
    menu_two->addAction("基站一");
    menu_two->addAction("基站二");
    menu_two->addAction("基站三");
    //connect点击菜单后对应的事件
    connect(menu_two,SIGNAL(triggered(QAction*)),this,SLOT(menu_two_trigged(QAction*)));

    //为辅助功能加界面
    menu_three = new QMenu();
    menu_three->addAction("看日志");
    connect(menu_three,SIGNAL(triggered(QAction*)),this,SLOT(menu_three_trigged(QAction*)));

    //调用IconHelper库去添加图标
    IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->btn_min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btn_max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btn_close, QChar(0xf00d));
    ui->widgetTitle->installEventFilter(this);//安装事件过滤器
    ui->widgetTitle->setProperty("form","title");//设置属性，与qss文件中的样式对应
    ui->widgetTop->setProperty("nav","top");
    ui->labTitle->setFont(QFont("Microsoft Yahei", 20));//设置标题字体

    QSize icoSize(32,32);//图标size
    int icoWidth=85;//按钮宽度
    //遍历widgetTop中所有的按钮，并设置属性
    QList<QToolButton *> btns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns){
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        btn->setPopupMode(QToolButton::InstantPopup);//按下工具按钮时菜单显示，无延迟。这种模式下，按钮自身的动作不触发。
    }
    ui->btnBs->setIconSize(QSize(42, 42));
    ui->btnMg->setMenu(menu_one);
    ui->btnBs->setMenu(menu_two);
    ui->btnConfig->setMenu(menu_three);

    //画xmap图
    // configure axis rect:
    QCustomPlot *customPlot=ui->xmap_wid;
    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    //设置xmap图size
    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    int nx = 28;
    int ny = 28;
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(0, nx), QCPRange(0, ny)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:

    QFile file("/home/nano/result.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QVector<int> heat_zlist;
    for(int k = 0;k<28;k++)
    {
        QByteArray Line =file.readLine();
        QString str(Line);
        for(int i=0;i<28;i++)
        {
            QString str1(str.section(' ',i,i).trimmed());
            double str1_int = str1.toDouble();
            heat_zlist.push_back(str1_int);
         }
     }


     for (int xIndex=0; xIndex<nx; ++xIndex)
     {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            //colorMap->data()->cellToCoord(xIndex, yIndex, &xIndex, &yIndex);
            //double r = 3*qSqrt(x*x+y*y)+1e-2;
            //z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
            colorMap->data()->setCell(yIndex, xIndex, heat_zlist[28*xIndex+yIndex]);
        }
     }

     //颜色滑动框
     // add a color scale:
     QCPColorScale *colorScale = new QCPColorScale(customPlot);
     customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
     colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
     colorMap->setColorScale(colorScale); // associate the color map with the color scale
     colorScale->axis()->setLabel("Magnetic Field Strength");

     // set the color gradient of the color map to one of the presets:
     colorMap->setGradient(QCPColorGradient::gpSpectrum);
     // we could have also created a QCPColorGradient instance and added own colors to
     // the gradient, see the documentation of QCPColorGradient for what's possible.

     // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
     colorMap->rescaleDataRange();

     // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
     QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
     customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
     colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

     // rescale the key (x) and value (y) axes so the whole color map is visible:
     customPlot->rescaleAxes();

     //网格在上层，图像在下层
     customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
     customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
     colorMap->setLayer("belowmain");
     customPlot->xAxis->grid()->setLayer("abovemain");
     customPlot->yAxis->grid()->setLayer("abovemain");
     customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
     customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
     customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
     customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
     customPlot->xAxis->grid()->setSubGridVisible(true);
     customPlot->yAxis->grid()->setSubGridVisible(true);
     customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
     customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);



}

//点击管理方案的菜单
void Widget::menu_one_trigged(QAction *action)
{
    qDebug()<<"hello "<<action->text();
    QString name = action->text();
    //自配置&窗口没打开
    if(name=="自配置"&&cg_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>准备配置基站</font>"));
        //标识符设为1，表示窗口打开
        cg_i=1;
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        //新建config_mainwindow窗口
        cg_frame = new config_mainwindow(server, this);
        cg_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        cg_frame->setWindowTitle("自配置");
        //connect关闭窗口对应的事件
        connect(cg_frame,SIGNAL(close_cg()),this,SLOT(cg_close()));
        cg_frame->move(((wd-600)/2),(ht-400)/2);//窗口居中
        cg_frame->setFixedSize(600,400);//去除最大化按钮
        cg_frame->show();
        connect(cg_frame, SIGNAL(emit_confeNb(QString, qintptr)), this, SLOT(transferData(QString, qintptr)));
        connect(cg_frame,SIGNAL(emit_to_main(QString)),this,SLOT(to_main(QString)));
    }
    //自优化&窗口没打开
    if(name=="自优化"&&opt_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>准备自优化</font>"));
        //标识符设为1，表示窗口打开
        opt_i=1;
        opt_frame = new optimize_mainwindow(this);
        opt_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        opt_frame->setWindowTitle("自优化");
        connect(opt_frame,SIGNAL(close_opt()),this,SLOT(opt_close()));
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        opt_frame->move(((wd-600)/2),(ht-400)/2);//窗口居中
        opt_frame->setFixedSize(600,400);//去除最大化按钮
        opt_frame->show();
    }
    if(name=="自治愈"&&heal_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>准备自治愈</font>"));
        //标识符设为1，表示窗口打开
        heal_i=1;
        heal_frame = new heal_mainwindow(this);
        heal_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        heal_frame->setWindowTitle("自治愈");
        connect(heal_frame,SIGNAL(close_heal()),this,SLOT(heal_close()));
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        heal_frame->move(((wd-600)/2),(ht-400)/2);//窗口居中
        heal_frame->setFixedSize(600,400);//去除最大化按钮
        heal_frame->show();
    }
}

//自配置窗口关闭
void Widget::cg_close()
{
    cg_i=0;
}

void Widget::opt_close()
{
    opt_i=0;
}

void Widget::heal_close()
{
    heal_i=0;
}

//点击配置基站的菜单
void Widget::menu_two_trigged(QAction *action)
{
    qDebug()<<"hello "<<action->text();
    QString name = action->text();
    if(name=="基站一"&&bsone_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>设置基站一</font>"));
        //标识符设为1，表示窗口打开
        bsone_i=1;
        bsone_frame = new bsone_mainwindow(this);
        bsone_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        bsone_frame->setWindowTitle("基站一");
        connect(bsone_frame,SIGNAL(close_bsone()),this,SLOT(bsone_close()));
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        bsone_frame->move(((wd-400)/2),(ht-200)/2);//窗口居中
        bsone_frame->setFixedSize(400,200);//去除最大化按钮
        bsone_frame->show();
    }
    if(name=="基站二"&&bstwo_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>设置基站二</font>"));
        //标识符设为1，表示窗口打开
        bstwo_i=1;
        bstwo_frame = new bstwo_mainwindow(this);
        bstwo_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        bstwo_frame->setWindowTitle("基站二");
        connect(bstwo_frame,SIGNAL(close_bstwo()),this,SLOT(bstwo_close()));
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        bstwo_frame->move(((wd-400)/2),(ht-200)/2);//窗口居中
        bstwo_frame->setFixedSize(400,200);//去除最大化按钮
        bstwo_frame->show();
    }
    if(name=="基站三"&&bsthree_i==0){
        //消息输出框显示反馈
        ui->textBrowser->append(QString("<font color=black>设置基站三</font>"));
        //标识符设为1，表示窗口打开
        bsthree_i=1;
        bsthree_frame = new bsthree_mainwindow(this);
        bsthree_frame->setProperty("config","white");//设置属性，与qss文件中的样式对应
        bsthree_frame->setWindowTitle("基站三");
        connect(bsthree_frame,SIGNAL(close_bsthree()),this,SLOT(bsthree_close()));
        QDesktopWidget *desk=QApplication::desktop();
        int wd=desk->width();
        int ht=desk->height();
        bsthree_frame->move(((wd-400)/2),(ht-200)/2);//窗口居中
        bsthree_frame->setFixedSize(400,200);//去除最大化按钮
        bsthree_frame->show();

    }

}

void Widget::bsone_close()
{
    bsone_i=0;
}

void Widget::bstwo_close()
{
    bstwo_i=0;
}

void Widget::bsthree_close()
{
    bsthree_i=0;
}

void Widget::menu_three_trigged(QAction *action)
{
    qDebug()<<"hello "<<action->text();
    QString name = action->text();
}


//点击最大化按钮
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

//点击最小化按钮
void Widget::on_btn_min_clicked()
{
    showMinimized();
}

//点击关闭按钮
void Widget::on_btn_close_clicked()
{
    this->close();
}

//与emit_to_main函数对应，给消息输出框添加反馈
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
