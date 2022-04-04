#include "widget.h"
#include "ui_widget.h"
#include "elevator.h"
#include <QPushButton>
#include <QString>
#include <QSlider>
#include <QFile>
#include <QTime>
#include <QSignalMapper>
#include "elevatorcontroller.h"

void delay_2(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    void (QLCDNumber::* slot1)(int)  = &QLCDNumber::display;
    ui->verticalSlider_1->setValue(1);
    ui->verticalSlider_2->setValue(1);
    ui->verticalSlider_3->setValue(1);
    ui->verticalSlider_4->setValue(1);
    ui->verticalSlider_5->setValue(1);
    ui->lcdNumber_1->display(1);
    ui->lcdNumber_2->display(1);
    ui->lcdNumber_3->display(1);
    ui->lcdNumber_4->display(1);
    ui->lcdNumber_5->display(1);
    QObject::connect(ui->verticalSlider_1, &QSlider::valueChanged, ui->lcdNumber_1, slot1);
    QObject::connect(ui->verticalSlider_2, &QSlider::valueChanged, ui->lcdNumber_2, slot1);
    QObject::connect(ui->verticalSlider_3, &QSlider::valueChanged, ui->lcdNumber_3, slot1);
    QObject::connect(ui->verticalSlider_4, &QSlider::valueChanged, ui->lcdNumber_4, slot1);
    QObject::connect(ui->verticalSlider_5, &QSlider::valueChanged, ui->lcdNumber_5, slot1);

    this->elevator = new Elevator[5];
    elevator[0].lcd = ui->lcdNumber_1;
    elevator[0].innerBtn[0] = ui->pushButton11;
    elevator[0].innerBtn[1] = ui->pushButton12;
    elevator[0].innerBtn[2] = ui->pushButton13;
    elevator[0].innerBtn[3] = ui->pushButton14;
    elevator[0].innerBtn[4] = ui->pushButton15;
    elevator[0].innerBtn[5] = ui->pushButton16;
    elevator[0].innerBtn[6] = ui->pushButton17;
    elevator[0].innerBtn[7] = ui->pushButton18;
    elevator[0].innerBtn[8] = ui->pushButton19;
    elevator[0].innerBtn[9] = ui->pushButton110;
    elevator[0].innerBtn[10] = ui->pushButton111;
    elevator[0].innerBtn[11] = ui->pushButton112;
    elevator[0].innerBtn[12] = ui->pushButton113;
    elevator[0].innerBtn[13] = ui->pushButton114;
    elevator[0].innerBtn[14] = ui->pushButton115;
    elevator[0].innerBtn[15] = ui->pushButton116;
    elevator[0].innerBtn[16] = ui->pushButton117;
    elevator[0].innerBtn[17] = ui->pushButton118;
    elevator[0].innerBtn[18] = ui->pushButton119;
    elevator[0].innerBtn[19] = ui->pushButton120;

    elevator[1].lcd = ui->lcdNumber_2;
    elevator[1].innerBtn[0] = ui->pushButton21;
    elevator[1].innerBtn[1] = ui->pushButton22;
    elevator[1].innerBtn[2] = ui->pushButton23;
    elevator[1].innerBtn[3] = ui->pushButton24;
    elevator[1].innerBtn[4] = ui->pushButton25;
    elevator[1].innerBtn[5] = ui->pushButton26;
    elevator[1].innerBtn[6] = ui->pushButton27;
    elevator[1].innerBtn[7] = ui->pushButton28;
    elevator[1].innerBtn[8] = ui->pushButton29;
    elevator[1].innerBtn[9] = ui->pushButton210;
    elevator[1].innerBtn[10] = ui->pushButton211;
    elevator[1].innerBtn[11] = ui->pushButton212;
    elevator[1].innerBtn[12] = ui->pushButton213;
    elevator[1].innerBtn[13] = ui->pushButton214;
    elevator[1].innerBtn[14] = ui->pushButton215;
    elevator[1].innerBtn[15] = ui->pushButton216;
    elevator[1].innerBtn[16] = ui->pushButton217;
    elevator[1].innerBtn[17] = ui->pushButton218;
    elevator[1].innerBtn[18] = ui->pushButton219;
    elevator[1].innerBtn[19] = ui->pushButton220;

    elevator[2].lcd = ui->lcdNumber_3;
    elevator[2].innerBtn[0] = ui->pushButton31;
    elevator[2].innerBtn[1] = ui->pushButton32;
    elevator[2].innerBtn[2] = ui->pushButton33;
    elevator[2].innerBtn[3] = ui->pushButton34;
    elevator[2].innerBtn[4] = ui->pushButton35;
    elevator[2].innerBtn[5] = ui->pushButton36;
    elevator[2].innerBtn[6] = ui->pushButton37;
    elevator[2].innerBtn[7] = ui->pushButton38;
    elevator[2].innerBtn[8] = ui->pushButton39;
    elevator[2].innerBtn[9] = ui->pushButton310;
    elevator[2].innerBtn[10] = ui->pushButton311;
    elevator[2].innerBtn[11] = ui->pushButton312;
    elevator[2].innerBtn[12] = ui->pushButton313;
    elevator[2].innerBtn[13] = ui->pushButton314;
    elevator[2].innerBtn[14] = ui->pushButton315;
    elevator[2].innerBtn[15] = ui->pushButton316;
    elevator[2].innerBtn[16] = ui->pushButton317;
    elevator[2].innerBtn[17] = ui->pushButton318;
    elevator[2].innerBtn[18] = ui->pushButton319;
    elevator[2].innerBtn[19] = ui->pushButton320;

    elevator[3].lcd = ui->lcdNumber_4;
    elevator[3].innerBtn[0] = ui->pushButton41;
    elevator[3].innerBtn[1] = ui->pushButton42;
    elevator[3].innerBtn[2] = ui->pushButton43;
    elevator[3].innerBtn[3] = ui->pushButton44;
    elevator[3].innerBtn[4] = ui->pushButton45;
    elevator[3].innerBtn[5] = ui->pushButton46;
    elevator[3].innerBtn[6] = ui->pushButton47;
    elevator[3].innerBtn[7] = ui->pushButton48;
    elevator[3].innerBtn[8] = ui->pushButton49;
    elevator[3].innerBtn[9] = ui->pushButton410;
    elevator[3].innerBtn[10] = ui->pushButton411;
    elevator[3].innerBtn[11] = ui->pushButton412;
    elevator[3].innerBtn[12] = ui->pushButton413;
    elevator[3].innerBtn[13] = ui->pushButton414;
    elevator[3].innerBtn[14] = ui->pushButton415;
    elevator[3].innerBtn[15] = ui->pushButton416;
    elevator[3].innerBtn[16] = ui->pushButton417;
    elevator[3].innerBtn[17] = ui->pushButton418;
    elevator[3].innerBtn[18] = ui->pushButton419;
    elevator[3].innerBtn[19] = ui->pushButton420;


    elevator[4].lcd = ui->lcdNumber_5;
    elevator[4].innerBtn[0] = ui->pushButton51;
    elevator[4].innerBtn[1] = ui->pushButton52;
    elevator[4].innerBtn[2] = ui->pushButton53;
    elevator[4].innerBtn[3] = ui->pushButton54;
    elevator[4].innerBtn[4] = ui->pushButton55;
    elevator[4].innerBtn[5] = ui->pushButton56;
    elevator[4].innerBtn[6] = ui->pushButton57;
    elevator[4].innerBtn[7] = ui->pushButton58;
    elevator[4].innerBtn[8] = ui->pushButton59;
    elevator[4].innerBtn[9] = ui->pushButton510;
    elevator[4].innerBtn[10] = ui->pushButton511;
    elevator[4].innerBtn[11] = ui->pushButton512;
    elevator[4].innerBtn[12] = ui->pushButton513;
    elevator[4].innerBtn[13] = ui->pushButton514;
    elevator[4].innerBtn[14] = ui->pushButton515;
    elevator[4].innerBtn[15] = ui->pushButton516;
    elevator[4].innerBtn[16] = ui->pushButton517;
    elevator[4].innerBtn[17] = ui->pushButton518;
    elevator[4].innerBtn[18] = ui->pushButton519;
    elevator[4].innerBtn[19] = ui->pushButton520;

    this->outerBtn[0][0]=ui->pushButton01;
    this->outerBtn[1][0]=ui->pushButton02;
    this->outerBtn[2][0]=ui->pushButton03;
    this->outerBtn[3][0]=ui->pushButton04;
    this->outerBtn[4][0]=ui->pushButton05;
    this->outerBtn[5][0]=ui->pushButton06;
    this->outerBtn[6][0]=ui->pushButton07;
    this->outerBtn[7][0]=ui->pushButton08;
    this->outerBtn[8][0]=ui->pushButton09;
    this->outerBtn[9][0]=ui->pushButton010;

    //this->outerBtn[0][1]=ui->pushButton011;
    this->outerBtn[1][1]=ui->pushButton012;
    this->outerBtn[2][1]=ui->pushButton013;
    this->outerBtn[3][1]=ui->pushButton014;
    this->outerBtn[4][1]=ui->pushButton015;
    this->outerBtn[5][1]=ui->pushButton016;
    this->outerBtn[6][1]=ui->pushButton017;
    this->outerBtn[7][1]=ui->pushButton018;
    this->outerBtn[8][1]=ui->pushButton019;
    this->outerBtn[9][1]=ui->pushButton020;

    this->outerBtn[10][0]=ui->pushButton021;
    this->outerBtn[11][0]=ui->pushButton022;
    this->outerBtn[12][0]=ui->pushButton023;
    this->outerBtn[13][0]=ui->pushButton024;
    this->outerBtn[14][0]=ui->pushButton025;
    this->outerBtn[15][0]=ui->pushButton026;
    this->outerBtn[16][0]=ui->pushButton027;
    this->outerBtn[17][0]=ui->pushButton028;
    this->outerBtn[18][0]=ui->pushButton029;
    //this->outerBtn[19][0]=ui->pushButton030;

    this->outerBtn[10][1]=ui->pushButton031;
    this->outerBtn[11][1]=ui->pushButton032;
    this->outerBtn[12][1]=ui->pushButton033;
    this->outerBtn[13][1]=ui->pushButton034;
    this->outerBtn[14][1]=ui->pushButton035;
    this->outerBtn[15][1]=ui->pushButton036;
    this->outerBtn[16][1]=ui->pushButton037;
    this->outerBtn[17][1]=ui->pushButton038;
    this->outerBtn[18][1]=ui->pushButton039;
    this->outerBtn[19][1]=ui->pushButton040;


    this->elevator[0].slide = ui->verticalSlider_1;
    this->elevator[1].slide = ui->verticalSlider_2;
    this->elevator[2].slide = ui->verticalSlider_3;
    this->elevator[3].slide = ui->verticalSlider_4;
    this->elevator[4].slide = ui->verticalSlider_5;


    for(int i=0;i<5;i++)
    {
        for(int j=0;j<20;j++)
        {
            connect(this->elevator[i].innerBtn[j],&QPushButton::clicked,&this->controller,[=]{this->controller.inBtnPressed(i,j);});
        }
    }

    for(int i=0;i<20;i++)
    {
        connect(this->outerBtn[i][0],&QPushButton::clicked,&this->controller,[=]{this->controller.outBtnPressed(i,0);});
        connect(this->outerBtn[i][1],&QPushButton::clicked,&this->controller,[=]{this->controller.outBtnPressed(i,1);});
    }
}

void Widget::updateUI()
{
    for(int i=0;i<5;i++)
    {
        this->elevator[i].slide->setValue(this->controller.currentFloor[i]+1);//按控制器中数组移动滑块
        for(int i=0;i<5;i++)//设置按钮颜色
        {
            for(int j=0;j<20;j++)
            {
                if(this->controller.inRequest[i][j] == 1)
                {
                    this->elevator[i].innerBtn[j]->setStyleSheet("background-color: rgb(170, 0, 255);");
                }
                else if(this->controller.inRequest[i][j] == 2)
                {
                    this->elevator[i].innerBtn[j]->setStyleSheet("background-color: rgb(255, 0, 0);");
                }
                else if(this->controller.currentFloor[i] == j)
                {
                    this->elevator[i].innerBtn[j]->setStyleSheet("background-color: rgb(100, 100, 100);");
                }
                else
                {
                    this->elevator[i].innerBtn[j]->setStyleSheet("background-color: rgb(255, 255, 255);");
                }
            }
        }
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<2;j++)
        {
            if( !((i==0&&j==1)||(i==19&&j==0)))
            {
                if(this->controller.outRequest[i][j])
                {
                    this->outerBtn[i][j]->setStyleSheet("background-color: rgb(170, 0, 255);");
                }
                else
                {
                    this->outerBtn[i][j]->setStyleSheet("background-color: rgb(255, 255, 255);");
                }
            }
        }
    }
}

void Widget::run()
{
    while(1)
    {
        this->controller.outToIn();
        this->controller.update();
        this->updateUI();
        delay_2(50);
    }
}

Widget::~Widget()
{
    delete ui;
}

