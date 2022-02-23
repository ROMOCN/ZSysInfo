#ifndef MY_GROUPBOX_H
#define MY_GROUPBOX_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
class My_GroupBox : public QGroupBox
{
    Q_OBJECT
    bool _empty = true;
    QVBoxLayout *layout = nullptr;
    int _rowNum = 0, _colunmNum = 0;
public:
    explicit My_GroupBox(QWidget *parent = nullptr);
    bool addWidget(QWidget *w);
    void leaveWidget();
    bool empty(){
        return _empty;
    }
    int rowNum(){
        return _rowNum;
    }
    int colunmNum(){
        return _colunmNum;
    }
    void setNum(int r, int c){
        setRow(r);
        setColunmNum(c);
    }
    void setRow(int r){
        _rowNum = r;
    }
    void setColunmNum(int c){
        _colunmNum = c;
    }
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void childEvent(QChildEvent *event) override;
signals:
    void signalFull(int r, int c);
};

#endif // MY_GROUPBOX_H
