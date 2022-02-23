#ifndef MY_EDIT_H
#define MY_EDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QMouseEvent>
#include <windows.h>
class My_Edit : public QTextEdit
{
public:
    My_Edit(QWidget *parent = nullptr) : QTextEdit(parent)
    {
        this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
             WS_EX_TRANSPARENT//忽略一切消息（WM_PAINT除外）
             | WS_EX_LAYERED); //层风格，有他才能支持半透明

    }
    void mouseMoveEvent(QMouseEvent *e) override
    {
        QWidget::mouseMoveEvent(e);
    }
};

#endif // MY_EDIT_H
