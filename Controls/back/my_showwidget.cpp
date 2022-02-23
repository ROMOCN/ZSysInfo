#include "my_showwidget.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>

My_ShowWidget::My_ShowWidget(QWidget *parent) : QWidget(parent)
{
    //this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
    this->resize(132,30);
    //this->setCursor(QCursor(Qt::PointingHandCursor)); 改变鼠标形状
    _labelName = new My_Edit(this);
    _labelValue = new My_Edit(this);
    _font = QFont( "Microsoft YaHei", 12, QFont::Bold );

    editInit(_labelName);
    editInit(_labelValue);
    //setFontSize(20);
    _labelName->resize(60,38);
    _labelName->move(0,-3);
    _labelValue->move(_labelName->width(), -3);
    _labelValue->resize(72,38);

//    this->setStyleSheet("background-color: red;");
//    _labelName->setStyleSheet("background-color: red;");
//    _labelValue->setStyleSheet("background-color: blue;");
}

void My_ShowWidget::setName(QString name)
{
//    editAddEdge(_labelValue);
    _labelName->setText(name);
}

void My_ShowWidget::setValue(int value)
{
//    editAddEdge(_labelValue);
    QString svalue = QString("%1%").arg(value);
    _labelValue->setText(svalue);
    _labelValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

//    setAlign(_labelValue);

}

void My_ShowWidget::setWidget(QString name, int value)
{
    this->setName(name);
    this->setValue(value);
}

void My_ShowWidget::setColor(QColor color)
{

}

void My_ShowWidget::setAlign(QTextEdit *edit)
{
    QTextCursor cursor = edit->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignRight | Qt::AlignVCenter);//or another alignment
    cursor.mergeBlockFormat(textBlockFormat);
    edit->setTextCursor(cursor);
}

void My_ShowWidget::setTransparent(int trans)
{

}

void My_ShowWidget::setFontBorder(bool yes)
{

}

void My_ShowWidget::setFontSize(int size)
{
    _fontsize = size;
//    _font = QFont( "Microsoft YaHei", size, QFont::Bold);
    _font.setPointSize(size);
    _labelName->setFont(_font);
    _labelValue->setFont(_font);

}

void My_ShowWidget::setFontFamily(QString family)
{
    _font.setFamily(family);
    _labelName->setFont(_font);
    _labelValue->setFont(_font);
}



void My_ShowWidget::editInit(QTextEdit *edit)
{
    //! 设置文本输入框无边框、背景透明、字体颜色和字体加粗，使得qtextedit和qlabel具有同样效果
    edit->setStyleSheet("border: none; background-color: transparent; color: rgb(0,0,0); ");
    edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    edit->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff ) ;
    edit->setReadOnly(true);
    edit->setFont(_font);
    edit->setTextInteractionFlags ( Qt::NoTextInteraction );
//    edit->set
//    edit->setDisabled(true);
    //! 设置字体黑色描边
//    editAddEdge(edit);
    //    edit->setText(text);
}

void My_ShowWidget::editAddEdge(QTextEdit *edit)
{
    QTextCharFormat format;
    format.setTextOutline(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    edit->mergeCurrentCharFormat(format);

}



void My_ShowWidget::resizeEvent(QResizeEvent *event)
{

}

void My_ShowWidget::paintEvent(QPaintEvent *event)
{

}

void My_ShowWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    //QWidget::mouseMoveEvent(event);
    if(mousePressed)
    {
        QPoint m = pointToParent + event->globalPos() - point;
        //this->move(m);
        emit signalWidgetMove(event, this,  m);

//        qDebug()<<"move:"<<event->globalPos()<<"  press:"<<point<<"  result:"<<m;
    }
}

void My_ShowWidget::mousePressEvent(QMouseEvent *event)
{
//    if(!mousePressed)
    event->ignore();

    point = event->globalPos();
    pointToParent = QWidget::pos();
    //qDebug()<<"press:" << point;
    mousePressed = true;

    qDebug()<<"QCursor:" << QCursor::pos();
    qDebug()<<"pos:" << event->pos();
    qDebug()<<"globalPos:" << point;
    qDebug()<<"mapToParent:" << mapToParent(event->pos());
    qDebug()<<"QWidget:" << QWidget::pos();


}
void My_ShowWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    if(mousePressed)
    {
        // 创建数据
//        QDrag* drag = new QDrag(this);
//        QMimeData* mimeData = new QMimeData;
//        mimeData->setParent(this);
//        drag->setMimeData(mimeData);
//        // 启动拖放 start a drag
//        Qt::DropAction result = drag->exec(Qt::CopyAction | Qt::MoveAction);

        QWidget *action = QApplication::widgetAt(this->pos());
    }
    mousePressed = false;

    if(this->pos().x() < 0 || this->pos().y() <0 )
    {
        this->move(0, 0);
    }else if(this->pos().x() < 0 || this->pos().y() > ((QWidget*)this->parent())->height())
    {
        this->move(0, ((QWidget*)this->parent())->height() - this->height());
    }else if(this->pos().x() > ((QWidget*)this->parent())->width() || this->pos().y() > ((QWidget*)this->parent())->height())
    {
        this->move(((QWidget*)this->parent())->width() - this->width(), ((QWidget*)this->parent())->height() - this->height());
    }else if(this->pos().x() > ((QWidget*)this->parent())->width() || this->pos().y() <0 )
    {
        this->move(((QWidget*)this->parent())->width() - this->width(), 0);
    }
}

void My_ShowWidget::leaveEvent(QEvent *event)
{
    event->ignore();

    mousePressed = false;
}
