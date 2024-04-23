#include "vsqtopencv01.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vsqtopencv01 w;
    w.show();
    return a.exec();
}
