#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QTime>
#include <thread>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <map>
#include <QColorDialog>
#include "Tools/tool_system.h"
#include "Tools/tool_value.h"
#include "Tools/tool_nv.h"
#include "Controls/my_showwidget.h"
#include "Tools/Tool_Edge.h"
#include "Controls/my_tablegroup.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
//    SYSTEM_INFO::
    SystemInfo *_sys;
    bool editModel = false, _close = false, _moveEnable = false; //编辑模式
    ValueHelper cpuHelper, gpuHelper;
    My_ShowWidget *showCpu = nullptr,
    *showRam = nullptr,
    *showGpu = nullptr,
    *showFps = nullptr;
    My_TableGroup *tg;
    QFont _font;
    QSystemTrayIcon *_myIcon;
    QMenu *_myMenu;
    Tool_Edge edge;
    std::map<My_ShowWidget*, QPoint> Show_Widgets;
    void threadUpdate();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTop(bool top);
    void setFont(QString path);
    void slotUpdateCPU(int value){
        showCpu->setValue(value);
    }
    void slotUpdateGPU(int value){
        showGpu->setValue(value);
    }
    void slotUpdateRAM(int value){
        showRam->setValue(value);
    }
    void slotUpdateFPS(int value){
        showFps->setValue(value);
    }
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void ignoreMouseEvent(bool ignore);
    void setBackHide(bool hide);//设置背景透明
    void setColor();

signals:
    void signalRAMValueChanged(int value);
    void signalFPSChanged(int value);
private slots:
    void slotWidgetMove(QMouseEvent *event, QWidget *member, QPoint point);

private:
    Ui::MainWindow *ui;
    void iconInit();
    void menuInit();
    inline bool pointInside(QPoint point, QRect rect);//判断一个矩形的起点(左上角)是否在另一个矩形内
};
#endif // MAINWINDOW_H
