#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QTime>
#include <time.h>

#define INST_NUM 320

//延时函数
void delay(int msec=10)
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
    this->instruction = new int[320];
    this->init();//初始化

    //按钮点击事件与对应的槽函数
    connect(ui->btn_start,&QPushButton::clicked,this,
        [=]{
            this->init();
            this->generateList();
            this->showList();
            ui->label_choose->setVisible(true);
            ui->btn_FIFO->setVisible(true);
            ui->btn_LRU->setVisible(true);
            ui->btn_start->setText("重新开始");
        });
    connect(ui->btn_FIFO,&QPushButton::clicked,this,
            [=]{
                ui->btn_FIFO->setStyleSheet("background-color: rgb(238,130,238);");
                //ui->btn_LRU->setVisible(false);
                ui->btn_LRU->setEnabled(false);
                this->algoType=FIFO;
                ui->btn_oneStep->setVisible(true);
                ui->btn_tenStep->setVisible(true);
                ui->btn_toEnd->setVisible(true);
            });
    connect(ui->btn_LRU,&QPushButton::clicked,this,
            [=]{
                ui->btn_LRU->setStyleSheet("background-color: rgb(238,130,238);");
                //ui->btn_FIFO->setVisible(false);
                ui->btn_FIFO->setEnabled(false);
                this->algoType=LRU;
                ui->btn_oneStep->setVisible(true);
                ui->btn_tenStep->setVisible(true);
                ui->btn_toEnd->setVisible(true);
            });
    connect(ui->btn_oneStep,&QPushButton::clicked,this,&Widget::oneStep);
    connect(ui->btn_tenStep,&QPushButton::clicked,this,&Widget::tenStep);
    connect(ui->btn_toEnd,&QPushButton::clicked,this,&Widget::toEnd);
    connect(ui->btn_end,&QPushButton::clicked,this,&Widget::close);
}

void Widget::init(){
    //初始化页表
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("逻辑页号"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("内存块号"));
    for(int i=1;i<=32;i++){
        this->pageBlock[i]=-1;
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i-1)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(-1)));
    }

    //调度算法相关信息初始化
    this->currentInst = -1;
    this->faultCount = 0;
    for(int i=0;i<4;i++){
        this->statusFIFO[i]=0;
        this->statusLRU[i]=0;
        this->block[i] = -1;
    }
    for(int i=0;i<32;i++){
        this->pageBlock[i]=-1;
    }

    //列表初始化
    ui->listWidget_2->clear();
    ui->listWidget->clear();
    ui->listWidget_log->clear();
    ui->listWidget_block1->clear();
    ui->listWidget_block2->clear();
    ui->listWidget_block3->clear();
    ui->listWidget_block4->clear();

    //按钮初始化
    ui->btn_FIFO->setVisible(false);
    ui->btn_LRU->setVisible(false);
    ui->btn_oneStep->setVisible(false);
    ui->btn_tenStep->setVisible(false);
    ui->btn_toEnd->setVisible(false);
    ui->btn_FIFO->setStyleSheet("background-color: rgb(255,255,255);");
    ui->btn_LRU->setStyleSheet("background-color: rgb(255,255,255);");
    ui->btn_FIFO->setEnabled(true);
    ui->btn_LRU->setEnabled(true);

    //lcdNumber及标签初始化
    ui->lcdNumber_1->display("-1");
    ui->lcdNumber_2->display("-1");
    ui->lcdNumber_3->display("-1");
    ui->lcdNumber_4->display("-1");
    ui->label_inst->setText("0");
    ui->label_faults->setText("0");
    ui->label_rate->setText("\\");
    ui->label_choose->setVisible(false);

    //进度条初始化
    ui->progressBar->setValue(0);
}

void Widget::generateList(){
    srand(time(NULL)); //根据当前时间设置“随机数种子”
    int m = rand() % INST_NUM;
    for(int i=0;i<=79;i++){
        //避免m为0报错
        while(m==0){
            m = rand() % INST_NUM;
        }
        int m1 = rand() % m;
        instruction[i*4] = m1;
        instruction[i*4+1] = (m+1)%INST_NUM;
        while(m1==0){
            m1 = rand() % INST_NUM;
        }
        int m2 = m1+1+ rand() % (INST_NUM-1-m1);
        instruction[i*4+2] = m2;
        instruction[i*4+3] = m2+1;
        m = m1;
    }
}

void Widget::showList(){
    for(int i=0;i<INST_NUM;i++){
        QString addString1 = QString("%1").arg(instruction[i], 3, 10, QChar('0'));
        QString addString2 = QString::number(instruction[i]/10);
        ui->listWidget->addItem(addString1+"  ("+addString2+")");
    }
    //ui->listWidget->selectAll();
    ui->listWidget->setCurrentRow(0);
}

void Widget::oneStep(){
    if(currentInst==INST_NUM-1)//指令已经访问完
        return;
    this->currentInst++;
    ui->label_inst->setText(QString::number(currentInst+1));//显示当前以访问指令数量
    ui->listWidget->setCurrentRow(currentInst);//在ListWidget中显示当前访问指令
    if(this->algoType==FIFO){
        this->OneStepFIFO(instruction[currentInst]);
    }  else {
        this->OneStepLRU(instruction[currentInst]);
    }
    //更新缺页率
    float newRate = float(faultCount)/float(currentInst+1);
    ui->label_rate->setText(QString::number(newRate));
    ui->progressBar->setValue(currentInst+1);
}

void Widget::tenStep(){
    //避免此时按其他按钮运行错误
    ui->btn_start->setEnabled(false);
    ui->btn_oneStep->setEnabled(false);
    ui->btn_toEnd->setEnabled(false);

    int i=0;
    while(currentInst<INST_NUM-1 && i++<10){
        oneStep();
        delay();
    }

    ui->btn_start->setEnabled(true);
    ui->btn_oneStep->setEnabled(true);
    ui->btn_toEnd->setEnabled(true);
}

void Widget::toEnd(){
    //避免此时按其他按钮运行错误
    ui->btn_start->setEnabled(false);
    ui->btn_oneStep->setEnabled(false);
    ui->btn_tenStep->setEnabled(false);

    while(currentInst<INST_NUM-1){
        oneStep();
        delay();
    }

    ui->btn_start->setEnabled(true);
    ui->btn_oneStep->setEnabled(true);
    ui->btn_tenStep->setEnabled(true);
}

void Widget::OneStepFIFO(int inst){
    //检测当前指令是否放入内存
    for(int i=0;i<4;i++){
        if(block[i]==inst/10){
            setBlock(i,inst);
            return;
        }
    }
    //检测是否有空闲内存块,只有初始时才会出现此情况
    for(int i=0;i<4;i++){
        if(block[i]==-1){
            setBlock(i,inst);
            statusFIFO[i]=1;
            updateFault(inst);
            for(int j=0;j<4;j++){
                if(j!=i&&block[j]!=-1){
                    statusFIFO[j]+=1;
                }
            }
            return;
        }
    }
    //缺页中断，换出最先进入内存指令
    for(int i=0;i<4;i++){
        if(statusFIFO[i]!=4){
            statusFIFO[i]++;
        }else{
            setBlock(i,inst);
            updateFault(inst);
            statusFIFO[i]=1;
        }
    }
}

void Widget::OneStepLRU(int inst){
    //检测当前指令是否放入内存
    for(int i=0;i<4;i++){
        if(block[i]==inst/10){
            this->statusLRU[i]=0;//最近访问时间更新
            for(int j=0;j<4;j++){
                if(j!=i&&block[j]!=-1){
                    statusLRU[j]+=1;
                }
            }
            this->setBlock(i,inst);
            return;
        }
    }
    //检测是否有空闲内存块,只有初始时才会出现此情况
    for(int i=0;i<4;i++){
        if(block[i]==-1){
            setBlock(i,inst);
            statusFIFO[i]=1;
            updateFault(inst);
            for(int j=0;j<4;j++){
                if(j!=i&&block[j]!=-1){
                    statusLRU[j]+=1;
                }
            }
            return;
        }
    }
    //缺页中断，换出最近未被访问内存
    int maxIndex = 0;
    int maxTime = this->statusLRU[0];
    for(int i=1;i<4;i++){
        if(statusLRU[i]>maxTime){
            maxTime = statusLRU[i];
            maxIndex = i;
        }
    }
    for(int i=0;i<4;i++){
        if(i!=maxIndex){
            statusLRU[i]++;
        }else{
            setBlock(i,inst);
            updateFault(inst);
            statusLRU[i]=0;
        }
    }
}

void Widget::setBlock(int blockID, int inst){
    updateLog(blockID,inst);
    int page = inst/10;
    if(block[blockID]!=page){
        block[blockID] = page;
        if(blockID==0) ui->listWidget_block1->clear();
        if(blockID==1) ui->listWidget_block2->clear();
        if(blockID==2) ui->listWidget_block3->clear();
        if(blockID==3) ui->listWidget_block4->clear();
    }
    QString addString = QString("%1").arg(inst, 3, 10, QChar('0'));
    //QString addString = QString::number(inst);//待插入项
    int duplicate = false;//是否重复
    if(blockID==0){
            ui->lcdNumber_1->display(QString::number(page));
            for(int row = 0;row<ui->listWidget_block1->count();row++){
                if(ui->listWidget_block1->item(row)->text()==addString){
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate)
                ui->listWidget_block1->addItem(addString);
      }
    if(blockID==1){
            ui->lcdNumber_2->display(QString::number(page));
            for(int row = 0;row<ui->listWidget_block2->count();row++){
                if(ui->listWidget_block2->item(row)->text()==addString){
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate)
                ui->listWidget_block2->addItem(addString);
      }
    if(blockID==2){
            ui->lcdNumber_3->display(QString::number(page));
            for(int row = 0;row<ui->listWidget_block3->count();row++){
                if(ui->listWidget_block3->item(row)->text()==addString){
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate)
                ui->listWidget_block3->addItem(addString);
      }
    if(blockID==3){
            ui->lcdNumber_4->display(QString::number(page));
            for(int row = 0;row<ui->listWidget_block4->count();row++){
                if(ui->listWidget_block4->item(row)->text()==addString){
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate)
                ui->listWidget_block4->addItem(addString);
      }
}

void Widget::updateLog(int blockID, int inst){
    QString addString1 = QString("%1").arg(inst, 3, 10, QChar('0'));
    QString addString2 = QString::number(inst/10);
    QString addString3 = QString::number(blockID+1);
    QString addString4 = QString::number(block[blockID]);
    QString addLog = "访问指令"+addString1+"("+addString2+"). ";
    int page = inst/10;
    if(block[blockID]!=page){
        addLog+="缺页中断.";
        if(block[blockID]!=-1){
            addLog += "block"+addString3 + "原本页" +addString4 + "被换出，";
            ui->tableWidget->setItem(block[blockID]+1,1,new QTableWidgetItem(QString::number(-1)));
        }
        addLog += "页" +addString2 + "装入" + "block"+addString3;
        ui->tableWidget->setItem(inst/10+1,1,new QTableWidgetItem(QString::number(blockID+1)));
    } else{
        addLog += "在block"+addString3+"中";
    }
    ui->listWidget_log->addItem(addLog);
    ui->listWidget_log->setCurrentRow(currentInst);
}

void Widget::updateFault(int inst){
    //在缺页列表中加入对应指指令及其页号
    QString addString1 = QString("%1").arg(inst, 3, 10, QChar('0'));
    QString addString2 = QString::number(inst/10);
    ui->listWidget_2->addItem(addString1+"  ("+addString2+")");

    this->faultCount++;
    ui->listWidget_2->setCurrentRow(faultCount-1);//在缺页列表中显示当前缺页
    ui->label_faults->setText(QString::number(faultCount));
}

Widget::~Widget()
{
    delete [] instruction;
    delete ui;
}

