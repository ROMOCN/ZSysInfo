#ifndef MY_SHOWWIDGET_H
#define MY_SHOWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QTextEdit>
#include <QDebug>
#include "my_edit.h"
class My_ShowWidget: public QWidget
{
    Q_OBJECT
    My_Edit *_labelName= nullptr, *_labelValue = nullptr;
    QColor _fontColor= QColor(255,255,255,255);
    QFont _font;
    int _fontsize = 18;
    QTextEdit *text_value2 = nullptr;
    bool mousePressed = false;
    QPoint point, pointToParent;
public:
    My_ShowWidget(QWidget *parent = nullptr);
    void setName(QString name);
    void setValue(int value);
    void setWidget(QString name, int value);
    void setColor(QColor color);
    void setAlign(QTextEdit *edit);
    void setTransparent(int trans);
    void setFontBorder(bool yes);
    void setFontSize(int size);
    void setFontFamily(QString family);
    QString getName();
    int getValue();
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
signals:
    void signalWidgetMove(QMouseEvent *event, QWidget *member, QPoint point);
private:
    void editInit(QTextEdit *edit);
    void editAddEdge(QTextEdit *edit);
};

#endif // MY_SHOWWIDGET_H
