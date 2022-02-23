#ifndef MY_GROUPBOX_H
#define MY_GROUPBOX_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
class My_GroupBox : public QGroupBox
{
    Q_OBJECT
    bool empty = true;
public:
    explicit My_GroupBox(QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void childEvent(QChildEvent *event) override;
};

#endif // MY_GROUPBOX_H
