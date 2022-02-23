#ifndef TOOL_VALUE_H
#define TOOL_VALUE_H
#include <vector>
#include <QObject>



class ValueHelper : public QObject{
    Q_OBJECT
    protected:
    int _currentPos = 0;
    int _size = 0;
    int _sizeTemp = 0;
    int _value = 0;
    int _sum = 0;
public:
    ValueHelper(QObject *parent = nullptr): QObject(parent)
    {

    }
    ValueHelper(int size , QObject *parent = nullptr): QObject(parent)
    {
        this->reSize(size);
    }
    ValueHelper (const ValueHelper& cust, QObject *parent = nullptr): QObject(parent)
    {
        this->_sum = cust._sum;
        this->_size = cust._size;
        this->_value = cust._value;
        this->_currentPos = cust._currentPos;
        this->valueVector = cust.valueVector;
    }
    static int valueProject(int value){
        int result;
        if( value > 100){
            result = 100;
        }else if(value < 0){
            result = 0;
        }else{
            result = value;
        }
        return result;
    }

    void reSize(int size)
    {
        valueVector.clear();
        valueVector.resize(size);
        _size = size;
        _sizeTemp = size;
    }
    void pushBack(int value)
    {
        valueVector[_currentPos] = valueProject(value);
        _sum += valueProject(value);
        if(_currentPos + 1 < _size){
            _currentPos++;
        }
        else{
            _currentPos = 0;
            valueVector.clear();
            valueVector.resize(_sizeTemp);
            emit signalAverage( valueProject(_sum / _size) );
            _sum = 0;
        }
    }
    int Size(){
        return _size;
    }
    std::vector<int> valueVector;
   signals:
    void signalAverage(int value);
};
#endif // TOOL_VALUE_H
