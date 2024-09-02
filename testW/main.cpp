#include "testw.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestW w;
    w.show();
    return a.exec();
}
