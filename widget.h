#ifndef WIDGET_H
#define WIDGET_H

#include "elevator.h"
#include"elevatorcontroller.h"
#include <QPushButton>
#include <QWidget>
#include <QSlider>
#include <QSignalMapper>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    void updateUI();

    void run();

    ~Widget();

private:
    Ui::Widget *ui;
    Elevator* elevator;
    QPushButton* outerBtn[20][2];
    QSignalMapper* signalMapper;
    ElevatorController controller;
};
#endif // WIDGET_H
