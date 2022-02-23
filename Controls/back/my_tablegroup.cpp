#include "my_tablegroup.h"

My_TableGroup::My_TableGroup(QWidget *parent) : QGroupBox(parent)
{
    init();

}

void My_TableGroup::resizeEvent(QResizeEvent *event)
{

}

void My_TableGroup::init()
{
    layout  = new QGridLayout(this);
    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            My_GroupBox *box = new My_GroupBox(this);
            box->setStyleSheet(".My_TableGroup{background-color:blue;}");
            groups[i][j] = box;
            layout->addWidget(box ,i , j);
        }
    }
}
