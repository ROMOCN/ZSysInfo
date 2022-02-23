#include "my_groupbox.h"

#include <qevent.h>
#include <QDropEvent>
#include "my_showwidget.h"
#include <QPushButton>
My_GroupBox::My_GroupBox(QWidget *parent):QGroupBox(parent)
{
    this->setStyleSheet("background-color:red;");
    this->setAcceptDrops(true);
    this->lower();
    this->setObjectName("My_GroupBox");
}

void My_GroupBox::dropEvent(QDropEvent *event)
{
    My_ShowWidget *show = (My_ShowWidget*)event->source();
    show->move(this->pos());
    show->setParent(this);
}

void My_GroupBox::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void My_GroupBox::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void My_GroupBox::dragLeaveEvent(QDragLeaveEvent *event)
{

}
void My_GroupBox::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

}
void My_GroupBox::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

}
void My_GroupBox::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
}

void My_GroupBox::leaveEvent(QEvent *event)
{
    event->ignore();
}

void My_GroupBox::childEvent(QChildEvent *event)
{

}
