#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>

class Elevator //对应的了ui界面与电梯有关的部件指针
{
public:
    explicit Elevator();
    ~Elevator()
    {
        //delete [] this->innerBtn;
        //delete this->slide;
       // delete this->lcd;
    }

    QPushButton* innerBtn[20];//按钮指针
    QSlider * slide;//进度条指针
    QLCDNumber * lcd;//lcd管指针

private:

signals:

};

#endif // ELEVATOR_H
