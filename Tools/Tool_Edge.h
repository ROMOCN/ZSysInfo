#ifndef TOOL_EDGE_H
#define TOOL_EDGE_H
#include <QGuiApplication>
#include <QMouseEvent>
#include <QObject>
#include <QScreen>
#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
class Tool_Edge:public QWidget
{
    bool mousePressed = false;
    QPoint point;
    QRect screenRect;
    QWidget *father = nullptr;
    enum ENUM_POS
    {
        POS_TOPRIGHT = 0,
        POS_TOPMIDDLE,
        POS_TOPLEFT,
        POS_BOTTOMRIGHT,
        POS_BOTTOMMIDDLE,
        POS_BOTTOMLEFT
    };

public:
    Tool_Edge(QWidget *parent = nullptr): QWidget(parent)
    {
        init(parent);
    }
    void init(QWidget *parent)
    {
        father  = parent;
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect deskRect = QApplication::desktop()->availableGeometry();
            //获取主屏幕分辨率
        screenRect = QApplication::desktop()->screenGeometry();
        //获取屏幕数量
        int nScreenCount = QApplication::desktop()->screenCount();
        //screenRect = screen->availableGeometry() ;
    }
    void movePos(ENUM_POS pos)
    {
        switch (pos)
        {
        case POS_TOPRIGHT:
            father->move(0, 0);
            break;
        case POS_TOPMIDDLE:
            father->move((screenRect.width() - father->width())/2 , 0);
            break;
        case POS_TOPLEFT:
            father->move(screenRect.width() - father->width(), 0);
            break;
        case POS_BOTTOMRIGHT:
            father->move(0, screenRect.height() - father->height());
            break;
        case POS_BOTTOMMIDDLE:
            father->move((screenRect.width() - father->width())/2 , screenRect.height() - father->height());
            break;
        case POS_BOTTOMLEFT:
            father->move(screenRect.width() - father->width(), screenRect.height() - father->height());
            break;
        }
    }
    void mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
        {
            mousePressed = true;
            //获得鼠标按键位置相对窗口左上面的位置
            point = event->globalPos() - father->frameGeometry().topLeft();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {

        mousePressed = false;
        QPoint pfather = father->pos();
        if(pfather.x() < 0 && pfather.y() <0 )
        {
            father->move(0, 0);
        }else if(pfather.x() < 0 && pfather.y() > screenRect.height() - father->height())
        {
            father->move(0, screenRect.height() - father->height());
        }else if(pfather.x() > screenRect.width()- father->width() && pfather.y() > screenRect.height() - father->height())
        {
            father->move(screenRect.width() - father->width(), screenRect.height() - father->height());
        }else if(pfather.x() > screenRect.width()- father->width() && pfather.y() <0 )
        {
            father->move(screenRect.width() - father->width(), 0);
        }else if(pfather.y() < 0
                 && pfather.x()> screenRect.width()/2 - father->width()
                 && pfather.x()< screenRect.width()/2 + father->width())
        {
            father->move((screenRect.width() - father->width())/2 , 0);
        }else if(pfather.y() > screenRect.height() - father->height()
                 && pfather.x()> screenRect.width()/2 - father->width()
                 && pfather.x()< screenRect.width()/2 + father->width())
        {
            father->move((screenRect.width() - father->width())/2 , screenRect.height() - father->height());
        }


    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() == Qt::LeftButton)
        {
            if(mousePressed)
            {
                if(father->windowState() == Qt::WindowState::WindowMaximized)
                    father->showNormal();
                father->move(event->globalPos() - point);
            }
        }

    }

    void leaveEvent(QEvent *event)
    {
        mousePressed = false;

    }
};

#endif // TOOL_EDGE_H
