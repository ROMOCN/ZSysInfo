#include "my_groupbox.h"

#include <qevent.h>
#include <QDropEvent>
#include "my_showwidget.h"
#include <QPushButton>
My_GroupBox::My_GroupBox(QWidget *parent):QGroupBox(parent)
{
    this->setAcceptDrops(true);
    this->setObjectName("My_GroupBox");
    layout = new QVBoxLayout(this);
//    layout->setSpacing(0);
    layout->setMargin(0);
    this->setLayout(layout);

    //this->setContentsMargins(0,0,0,0);

}


bool My_GroupBox::addWidget(QWidget *w)
{
    QWidget *wi = this->findChild<My_ShowWidget*>();
    if(!wi)
    {
        _empty = true;
    }
    else
    {
        _empty = false;
    }

    if(_empty)
    {
        int bef = layout->count(), af = 0;
        layout->addWidget(w);//, 0, Qt::AlignCenter
        af = layout->count();
        w->setParent(this);
        _empty = false;
        //qDebug()<<QString("addWidget{row:%1; colunm:%2;}").arg(_rowNum).arg(_colunmNum);
        //emit signalEmptyChanged(empty(), _childName,_rowNum, _colunmNum);
        signalFull(_rowNum, _colunmNum);

    }
}

void My_GroupBox::leaveWidget()
{
    bool emp = layout->isEmpty();
    qDebug()<<emp;
    _empty = true;

}

void My_GroupBox::dropEvent(QDropEvent *event)
{
    addWidget((QWidget*)event->source());
    bool emp = layout->isEmpty();
    qDebug()<<emp;

}

void My_GroupBox::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget::dragMoveEvent(event);
    event->acceptProposedAction();
}

void My_GroupBox::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void My_GroupBox::dragLeaveEvent(QDragLeaveEvent *event)
{
    leaveWidget();
    //qDebug()<<QString("leaveWidget{row:%1; colunm:%2;}").arg(_rowNum).arg(_colunmNum);
}

void My_GroupBox::childEvent(QChildEvent *event)
{
    qDebug()<<"childEvent";
}
