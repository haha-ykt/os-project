#include "widget.h"
#include "elevator.h"
#include <QApplication>
#include <QFile>
#include <thread>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();
    w.run();


    return a.exec();
}
