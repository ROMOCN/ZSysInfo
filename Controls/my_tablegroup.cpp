#include "my_tablegroup.h"

My_TableGroup::My_TableGroup(QWidget *parent) : QGroupBox(parent)
{
    init();
    layout->setSpacing(0);
    layout->setMargin(0);
    //this->setContentsMargins(0,0,0,0);
    this->setStyleSheet("QGroupBox{border:none;}");
    //parent->setContentsMargins(0,0,0,0);

}

void My_TableGroup::resizeEvent(QResizeEvent *event)
{

}

void My_TableGroup::addWidget(QWidget *w)
{
    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            if(groups[j][i]->empty())
            {
                groups[j][i]->addWidget(w);
                this->resize((w->width() + 20) * 4 , w->height() * 4 +12);
                if(this->parent())
                {
                    ((QWidget*)this->parent())->resize(this->size());
                }
                return;
            }
        }
    }
}

void My_TableGroup::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

//    QWidget *parent = (QWidget*)this->parent();
//    QWidget::mouseMoveEvent(event);
//    QPoint pos = event->globalPos();
//    int width =pos.x() - this->mapToGlobal(this->pos()).x();
//    int parentH = parent->height();
//    if(width&& parentH)
//        parent->resize(width, parentH);

//    if((QWidget*)this->parent())
//    {
//
//        QPoint pos = event->globalPos();
//        int width =pos.x() - this->mapToGlobal(this->pos()).x();
//        int one = this->width()/4;
//        int parentH = ((QWidget*)this->parent())->height();

//        if(width > ((QWidget*)this->parent())->width() && width < this->width())
//        {
//            int w = width / one;
//            w = width % one > 0 ? w : w++;
//            ((QWidget*)this->parent())->resize(w * one, parentH);
//        }else if( width > this->width())
//        {
//            ((QWidget*)this->parent())->resize(this->width(), parentH);
//        }
//    }

}

void My_TableGroup::parentMouseMoveEvent(QMouseEvent *event)
{
    if((QWidget*)this->parent())
    {
        QWidget *parent = (QWidget*)this->parent();
        QWidget::mouseMoveEvent(event);
        QPoint pos = event->globalPos();
        int width =pos.x() - this->mapToGlobal(this->pos()).x();
        int parentH = parent->height();
        if(width&& parentH)
            parent->resize(width, parentH);


//        int one = this->width()/4;


//        if(width > parent->width() && width < this->width())
//        {
//            int w = width / one;
//            w = width % one > 0 ? w : w++;
//            parent->resize(w * one, parentH);
//        }else if( width > this->width())
//        {
//            parent->resize(this->width(), parentH);
//        }

        //delete parent;
    }
}

void My_TableGroup::setBackTransparent(bool tran)
{
    if(tran){
        this->setStyleSheet("QGroupBox{background-color:transparent;border:none;}");
    }else{
        this->setStyleSheet("QGroupBox{background-color:gray;border:1px solid white;}");
    }
}





void My_TableGroup::init()
{
    this->lower();
    layout  = new QGridLayout(this);

    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            My_GroupBox *box = new My_GroupBox(this);
            box->setStyleSheet(".My_TableGroup{background-color:blue;}");
            groups[i][j] = box;
            groups[i][j]->setNum(i,j);
            connect(groups[i][j], &My_GroupBox::signalFull, this, &My_TableGroup::slotChildFull);
            layout->addWidget(box ,i , j);
        }
    }
}

void My_TableGroup::slotChildFull(int r, int c)
{

}
