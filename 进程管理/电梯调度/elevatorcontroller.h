#ifndef ELEVATORCONTROLLER_H
#define ELEVATORCONTROLLER_H

#include <QWidget>
#include <QDebug>
#include <vector>

namespace Ui { class Widget; }

class ElevatorController: public QWidget//电梯调度器，只记录电梯的状态与调度电梯的算法
{
public:
    ElevatorController(QWidget* parent =nullptr)
    {
        this->setParent(parent);
        for(int j=0;j<20;j++)
        {
            this->outRequest[j][0]=0;
            this->outRequest[j][1]=0;
            for(int i=0;i<2;i++)
            {
                this->outServe[j][i]=-1;
            }
            for(int i=0;i<5;i++)
            {
                this->inRequest[i][j]=0;
            }
        }
        for(int i=0;i<5;i++)
        {
            this->currentFloor[i] = 0;
            this->destFloor[i] = 0;
            this->direction[i] = 0;
            this->inOpenStop[i]=0;
           // this->isWork[i] = 1;
            //this->outOpenStop[i]=0;

        }
    }

    //响应内部按钮按下的槽函数
    void inBtnPressed(int i,int j, int type=1)
    {
        this->inRequest[i][j] = type;//type为1时表示的是内部按钮按下

        if(this->direction[i]==0)
        {
            if(currentFloor[i]!=j)
                this->direction[i] = j-currentFloor[i]>0?1:-1;
        }

    }

    //相应外部按钮按下的槽函数
    void outBtnPressed(int i,int j)
    {
        this->outRequest[i][j]=1;

        this->outRequestTuple.push_back(i*10+j);//如20层向下的请求就是201

        //qDebug()<<i<<j;
    }

    //将外部请求转化为内部请求
    void outToIn()
    {
        int* eraseRequest = new int[outRequestTuple.size()];
        for(int i=0;i<outRequestTuple.size();i++)
        {
            eraseRequest[i]=0;
        }
        for(int iter=0;iter<outRequestTuple.size();iter++)
        {
            int outDestFloor = outRequestTuple[iter]/10;
            int outDirection = outRequestTuple[iter]% 10 ? 1:-1;

            bool continueSign = false;

            //查找有无空闲电梯
            for(int i=0;i<5;i++)
            {
                if(this->direction[i]==0)
                {
                    this->outRequest[outDestFloor][outRequestTuple[iter]% 10] = 2;
                    this->outServe[outDestFloor][outRequestTuple[iter]% 10] = iter;
                    this->inBtnPressed(i,outDestFloor,3);
                    eraseRequest[iter]=1;
                    break;
                    continueSign = true;
                }
            }
            if(continueSign)
                continue;

            //continueSign = false;
            //查找最近的电梯
            if(outDirection == 1) //电梯向上
            {
                int best =-1;
                int lowCost = 100;
                for(int i=0;i<5;i++)
                {
                    if(this->currentFloor[i]<=outDestFloor && this->direction[i] == 1)//顺路且通向
                    {
                        int cost = abs(outDestFloor - currentFloor[i]);
                        if (cost<lowCost)
                        {
                            lowCost = cost;
                            best = i;
                            //continueSign = true;
                        }
                    }
                }
                if (best!=-1)
                {
                    this->outRequest[outDestFloor][outRequestTuple[iter]% 10] = 2;
                    this->inBtnPressed(best,outDestFloor,3);
                    this->outServe[outDestFloor][outRequestTuple[iter]% 10] = iter;
                    eraseRequest[iter] = 1;
                    //this->outRequestTuple.erase(outRequestTuple.begin()+iter);
                }
            }
            else
            {
                int best =-1;
                int lowCost = 100;
                for(int i=0;i<5;i++)
                {
                    if(this->currentFloor[i]>=outDestFloor && this->direction[i] == -1)//顺路且通向
                    {
                        int cost = abs(outDestFloor - currentFloor[i]);
                        if (cost<lowCost)
                        {
                            lowCost = cost;
                            best = i;
                        }
                    }
                }
                if (best!=-1)
                {
                    this->outRequest[outDestFloor][outRequestTuple[iter]% 10] = 2;
                    this->inBtnPressed(best,outDestFloor,3);
                    this->outServe[outDestFloor][outRequestTuple[iter]% 10] = iter;
                    eraseRequest[iter] = 1;
                    //this->outRequestTuple.erase(outRequestTuple.begin()+iter);
                }
            }

        }
        for(int i=outRequestTuple.size();i>=0;i--)
        {
            if(eraseRequest[i]==1)
            {
                this->outRequestTuple.erase(outRequestTuple.begin()+i);
            }
        }
    }

    //更新各楼梯内部状态
    void update()
    {
        //电梯正在运行时
        for(int i=0;i<5;i++)
        {

            if(inOpenStop[i])//若电梯处在开门状态则不更新
            {
                this->inRequest[i][this->currentFloor[i]] = 0;
                //this->outRequest[]
                inOpenStop[i] = 0;
            }
            else
            {
                this->currentFloor[i] += this->direction[i];
                if(this->inRequest[i][this->currentFloor[i]] == 1 || this->inRequest[i][this->currentFloor[i]] == 3)//当前层有请求
                {
                    if(this->outServe[this->currentFloor[i]][0]==i)
                    {
                        this->outServe[this->currentFloor[i]][0] = -1;
                        this->outRequest[this->currentFloor[i]][0]=0;
                    }
                    if(this->outServe[this->currentFloor[i]][1]==i)
                    {
                        this->outServe[this->currentFloor[i]][0] = -1;
                        this->outRequest[this->currentFloor[i]][1]=0;
                    }


                    this->inRequest[i][this->currentFloor[i]] = 2;//内部请求置为2,表示处于开门状态
                    this->inOpenStop[i]=1;//当前层要开门

                    //检查同方向上与反方向上是否有其他请求
                    if(this->direction[i]==1)
                    {
                        bool breakSign = false;
                        for(int floor = this->currentFloor[i];floor<20;floor++)//检查同方向
                        {
                            if(this->inRequest[i][floor] == 1 || this->inRequest[i][floor] == 3)
                            {
                                //this->destFloor[i] = floor;
                                breakSign = true;
                                break;
                            }
                        }
                        if(!breakSign)//检查反方向
                        {
                            for(int floor = this->currentFloor[i];floor>=0;floor--)//检查通方向
                            {
                                if(this->inRequest[i][floor] == 1 || this->inRequest[i][floor] == 3)
                                {
                                    //this->destFloor[i] = floor;
                                    this->direction[i]=-1;
                                    breakSign = true;
                                    break;
                                }
                            }
                        }
                        if(!breakSign)
                            direction[i] = 0;//正反方向均无请求
                    }
                    else if(this->direction[i]==-1)
                    {
                        bool breakSign = false;
                        for(int floor = this->currentFloor[i];floor>=0;floor--)//检查通方向
                        {
                            if(this->inRequest[i][floor] == 1 || this->inRequest[i][floor] == 3)
                            {
                                //this->destFloor[i] = floor;
                                breakSign = true;
                                break;
                            }
                        }
                        if(!breakSign)//检查反方向
                        {
                            breakSign = false;
                            for(int floor = this->currentFloor[i];floor<20;floor++)//检查通方向
                            {
                                if(this->inRequest[i][floor] == 1 || this->inRequest[i][floor] == 3)
                                {
                                    //this->destFloor[i] = floor;
                                    this->direction[i]=1;
                                    breakSign = true;
                                    break;
                                }
                            }
                        }
                        if(!breakSign)
                            direction[i] = 0;//正反方向均无请求
                    }
                }
            }


        }
    }


    int inRequest[5][20];//内部请求
    int outRequest[20][2];//外部请求
    std::vector<int> outRequestTuple;//记录外部未被相应请求
    int currentFloor[5];//当前楼层
    int destFloor[5];
    int direction[5];//电梯运行方向
    int inOpenStop[5];//内部开门状态
    int outServe[20][2];//电梯
    //int isRun[5];//电梯是否工作

private:
};

#endif // ELEVATORCONTROLLER_H
