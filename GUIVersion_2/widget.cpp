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

//    QPalette palette1;
//    palette1.setColor(QPalette::Background, QColor(209,199,183));
//    ui->info_wid->setAutoFillBackground(true);
//    ui->info_wid->setPalette(palette1);
//    ui->xmap_wid->setAutoFillBackground(true);
//    ui->xmap_wid->setPalette(palette1);
//    ui->tip_wid->setAutoFillBackground(true);
//    ui->tip_wid->setPalette(palette1);

        // configure axis rect:
        QCustomPlot *customPlot=ui->xmap_wid;
        customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
        customPlot->axisRect()->setupFullAxesBox(true);
        customPlot->xAxis->setLabel("x");
        customPlot->yAxis->setLabel("y");
        //customPlot->xAxis->grid()->setLayer("belowmain");
        //customPlot->yAxis->grid()->setLayer("belowmain");


        // set up the QCPColorMap:
        QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
        int nx = 28;
        int ny = 28;
        colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
        colorMap->data()->setRange(QCPRange(0, nx), QCPRange(0, ny)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
        // now we assign some data, by accessing the QCPColorMapData instance of the color map:

        QFile file("/home/nano/result.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
     //  int hang = 0;
       // double str2_int = 0;
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

        /*for (int k=28;k<812;k=k+28)
        {
            heat_xlist.push_back(k);
            heat_ylist.push_back(k);
        }
    */
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
