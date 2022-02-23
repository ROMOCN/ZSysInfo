#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFontDatabase>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBackHide(true);
    //this->resize(132 * 4, 120);
    this->move(0,0);
    tg = new My_TableGroup(this);
    _sys =new SystemInfo();
    showCpu = new My_ShowWidget(this);
    showCpu->setName("CPU");
    showCpu->move(0,0);
    showRam = new My_ShowWidget(this);
    showRam->setName("RAM");
    showRam->move(0, showCpu->height());
    showGpu = new My_ShowWidget(this);
    showGpu->setName("GPU");
    showGpu->move(0, showRam->height() + showRam->y());
    showFps = new My_ShowWidget(this);
    showFps->setName("FPS");
    showFps->move(0, showGpu->height() + showGpu->y());
    showFps->hide();
    showCpu->setObjectName("showCpu");
    showRam->setObjectName("showRam");
    showFps->setObjectName("showFps");
    showGpu->setObjectName("showGpu");

    tg->addWidget(showCpu);
    tg->addWidget(showRam);
    //tg->addWidget(showFps);
    tg->addWidget(showGpu);

    Show_Widgets.insert(std::map<My_ShowWidget*, QPoint>::value_type(showCpu, showCpu->pos()));
    Show_Widgets.insert(std::map<My_ShowWidget*, QPoint>::value_type(showRam, showRam->pos()));
    Show_Widgets.insert(std::map<My_ShowWidget*, QPoint>::value_type(showGpu, showGpu->pos()));
    Show_Widgets.insert(std::map<My_ShowWidget*, QPoint>::value_type(showFps, showFps->pos()));

    setFont(":/font/HarmonyOS_Sans_SC_Black.ttf");
    cpuHelper.reSize(10);
    gpuHelper.reSize(10);
    setTop(1);
    ignoreMouseEvent(true);

//    QCustomPlot *plot = new QCustomPlot(this);
    std::thread time =  std::thread(&MainWindow::threadUpdate, this);
    time.detach();
    connect(&cpuHelper, &ValueHelper::signalAverage, this, &MainWindow::slotUpdateCPU);
    connect(&gpuHelper, &ValueHelper::signalAverage, this, &MainWindow::slotUpdateGPU);
    connect(this, &MainWindow::signalRAMValueChanged, this, &MainWindow::slotUpdateRAM);
    connect(this, &MainWindow::signalFPSChanged, this, &MainWindow::slotUpdateFPS);
    connect(showCpu, &My_ShowWidget::signalWidgetMove, this, &MainWindow::slotWidgetMove);
    connect(showRam, &My_ShowWidget::signalWidgetMove, this, &MainWindow::slotWidgetMove);
    connect(showGpu, &My_ShowWidget::signalWidgetMove, this, &MainWindow::slotWidgetMove);
    //ignoreMouseEvent(false);
    iconInit();
    //this->resize(tg->size());

    edge.init(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::threadUpdate()
{
    QTime time(QTime::currentTime());//
    int i_fps = 0;
    int cpu;
    int ram;
    int gpu;
    while(!_close)
    {
        _sys->GetRAM();
        _sys->GetCPU();
        _sys->GetFPS();
        _sys->GetGPU();
        ram = _sys->RAM;
        i_fps = _sys->FPS;
        cpu = _sys->CPU;
        gpu = _sys->GPU;
        cpuHelper.pushBack(cpu);
        gpuHelper.pushBack(gpu);
        emit signalRAMValueChanged(ram);
        emit signalFPSChanged(i_fps);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MainWindow::setTop(bool top)
{
    if (top)
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        show();
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        show();
    }
}

void MainWindow::setFont(QString path)
{
    //根据添加时id打印字体名
    int id = QFontDatabase::addApplicationFont(path);
    qDebug()<<"family:"<<QFontDatabase::applicationFontFamilies(id);
    QString family = QFontDatabase::applicationFontFamilies(id).first();
    //_font.setFamily(QFontDatabase::applicationFontFamilies(id).first());
    showCpu->setFontFamily(family);
    showRam->setFontFamily(family);
    showGpu->setFontFamily(family);
    showFps->setFontFamily(family);
}



void MainWindow::ignoreMouseEvent(bool ignore)
{
    if(ignore){
#ifdef Q_OS_LINUX
     XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
           0, NULL, 0, ShapeSet, YXBanded);
#endif
#ifdef Q_OS_WIN
     SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
          WS_EX_TRANSPARENT//忽略一切消息（WM_PAINT除外）
          | WS_EX_LAYERED); //层风格，有他才能支持半透明
     //this->setAttribute(Qt::WA_TransparentForMouseEvents,true);
#endif
    }else{
        SetWindowLong((HWND)winId(),GWL_EXSTYLE,GetWindowLong((HWND)winId(),GWL_EXSTYLE) &
                      (~WS_EX_TRANSPARENT));
    }

}

void MainWindow::setBackHide(bool hide)
{
    if(hide)
    {
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }else
    {
        this->setWindowFlags(Qt::WindowType::Window );
        this->setAttribute(Qt::WA_TranslucentBackground, false);
    }
}

void MainWindow::setColor()
{
    QColorDialog dia(this);
    this->connect(&dia, &QColorDialog::currentColorChanged,[=](const QColor &color){
        showCpu->setColor(color);
        showRam->setColor(color);
        showGpu->setColor(color);
    });
    dia.setWindowTitle("颜色");

    //dia.setCurrentColor(Qt::white);//常值传递const引用
    dia.setCurrentColor(QColor(47,216,53));//通过生成临时对象来设置RGB初始参数
    if(dia.exec() == QColorDialog::Accepted )
    {
        QColor color = dia.selectedColor();
        showCpu->setColor(color);
        showRam->setColor(color);
        showGpu->setColor(color);

        qDebug() << color;
        qDebug() << color.red();
        qDebug() << color.green();
        qDebug() << color.blue();
        qDebug() << color.hue();
        qDebug() << color.saturation();
        qDebug() << color.value();
    }
}


void MainWindow::slotWidgetMove(QMouseEvent *event, QWidget *member, QPoint point)
{
    if(editModel){
        //member->move(point);
        //        QWidget *action = nullptr;
        //        QPoint p = event->pos();
        //        action = QApplication::widgetAt(QCursor::pos());
        //        if(action!=nullptr &&action->objectName() == "My_GroupBox"){
        //            My_GroupBox *box = (My_GroupBox*)action;
        //            member->setParent(box);
        //            QPoint pbox = box->pos();
        //            member->move(pbox);

        //        }

    }

}

void MainWindow::menuInit()
{
    _myMenu = new QMenu(this);
    _myMenu->setMinimumWidth(180);
    QAction *_act_hide = new QAction("隐藏(&N)",this);
    QAction *_act_show = new QAction("显示(&X)",this);
    QAction *_act_option = new QAction("设置(&C)",this);
    QAction * _act_lock = new QAction("解锁(&R)",this);
    QAction *_act_close = new QAction("退出",this);
    QAction *act_color = new QAction("颜色", this);
    _act_close->setShortcut(Qt::Key_F1);

    //    this->connect(_act_color,&QAction::triggered,this,&MainWindow::set_color);
    this->connect(_act_hide,SIGNAL(triggered()),this,SLOT(hide()));
    this->connect(_act_show,&QAction::triggered,[=](){
        this->show();
        this->move(0,0);
    });
    this->connect(_act_lock,&QAction::triggered, [=](){
        _moveEnable = !_moveEnable;
        ignoreMouseEvent(!_moveEnable);
        tg->setBackTransparent(!_moveEnable);
    });
    //this->connect(_act_close,SIGNAL(triggered()),qApp,SLOT(quit()));  //不能使用close
    this->connect(_act_close,&QAction::triggered,[=](){
        _close = true;
        _myIcon->hide();
        qApp->quit();
    });  //不能使用close
    this->connect(act_color,&QAction::triggered,[=](){
        setColor();
    });

    _myMenu->addAction(_act_option);
    _myMenu->addAction(_act_lock);
    _myMenu->addSeparator();     //加入一个分离符
    _myMenu->addAction(act_color);
    _myMenu->addSeparator();     //加入一个分离符
    _myMenu->addAction(_act_hide);
    _myMenu->addAction(_act_show);
    _myMenu->addSeparator();     //加入一个分离符
    _myMenu->addAction(_act_close);

}
/**
 * @name MainWindow::rectInside
 * @功能 判断一个矩形的起点(左上角)是否在另一个矩形内
 * @QPoint point起点坐标
 * @QRect rect矩形
 */
inline bool MainWindow::pointInside(const QPoint point,const QRect rect)
{
    bool result = false;
    if(rect.isValid() && !point.isNull())
    {
        int x = point.x(), y = point.y();
        if(x > rect.x() && x < rect.x() + rect.width() && y > rect.y() && y < rect.y() + rect.height())
            result = true;
    }
    return result;
}
void MainWindow::iconInit()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        return;
    }
    menuInit();

    _myIcon = new QSystemTrayIcon(this);
    _myIcon->setIcon(QIcon(":/images/sys2.ico"));   //设置图标图片
    _myIcon->setToolTip("Z_Sys V1.0");    //托盘时，鼠标放上去的提示信息
    _myIcon->showMessage("SystemTrayIcon","Hi,This is my trayIcon",QSystemTrayIcon::Information,10000);
    _myIcon->setContextMenu(_myMenu);     //设置托盘上下文菜单
    _myIcon->show();
}

void MainWindow::leaveEvent(QEvent *event)
{
    if(_moveEnable)
        edge.leaveEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //tg->resize(this->size());
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(_moveEnable)
        edge.mouseMoveEvent(event);



}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(_moveEnable){
        edge.mousePressEvent(event);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(_moveEnable)
    edge.mouseReleaseEvent(event);
}
