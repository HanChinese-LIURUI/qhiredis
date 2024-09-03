#include "tcpclient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcpClient w;
    //w.show();
    return a.exec();
}
