#ifndef MY_TABLEGROUP_H
#define MY_TABLEGROUP_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include "my_groupbox.h"
class My_TableGroup : public QGroupBox
{
    Q_OBJECT
    QGridLayout *layout = nullptr;
    std::array<std::array<My_GroupBox*, 4>, 4> groups;
public:
    explicit My_TableGroup(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;

signals:
private:
    void init();

};

#endif // MY_TABLEGROUP_H
