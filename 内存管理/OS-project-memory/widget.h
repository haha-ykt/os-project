#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

enum AlgoType{FIFO,LRU};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void init();//初始化界面
    void generateList();//产生指令序列
    void showList();//显示指令序列
    void oneStep();//访问单条指令
    void tenStep();//访问10条指令
    void toEnd();//访问所有指令
    void OneStepFIFO(int inst);//FIFO单条指令
    void OneStepLRU(int inst);//LRU单条指令
    void setBlock(int blockID,int inst);//设置内存块对应页面
    void updateLog(int blockID,int inst);//更新日志信息与页表信息
    void updateFault(int inst);//更新缺页指令与缺页率
    ~Widget();

private:
    AlgoType algoType;//算法类型
    int* instruction;//指令数组
    int currentInst;//当前指令对应序号
    int faultCount;//缺页数量
    int block[4];//记录各内存块当前所存页
    int pageBlock[32];//页表中记录各页对应内存
    int statusFIFO[4];//记录FIFO算法中各内存块对应相应页的存入时间相对优先级
    int statusLRU[4];//记录LRU算法中各内存块未被使用时间
    Ui::Widget *ui;
};
#endif // WIDGET_H
