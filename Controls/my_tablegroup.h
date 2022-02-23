#ifndef MY_TABLEGROUP_H
#define MY_TABLEGROUP_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QGroupBox>
#include "my_showwidget.h"
#include "my_groupbox.h"
class My_TableGroup : public QGroupBox
{
    Q_OBJECT
    QGridLayout *layout = nullptr;
    std::array<std::array<My_GroupBox*, 4>, 4> groups;
public:
    explicit My_TableGroup(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void addWidget(QWidget *w);
    void mouseMoveEvent(QMouseEvent *event) override;
    void parentMouseMoveEvent(QMouseEvent *event);
    void setBackTransparent(bool tran);
signals:
private:
    void init();
    void slotChildFull(int r, int c);


};

#endif // MY_TABLEGROUP_H
