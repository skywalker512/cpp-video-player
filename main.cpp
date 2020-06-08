#include "Xplay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Xplay w;
    w.show();
    return a.exec();
}
