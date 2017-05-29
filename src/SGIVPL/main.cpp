#include "SGIVPL\MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sgivpl::MainWindow w;
    w.show();
    return a.exec();
}
