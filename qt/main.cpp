#include <QApplication>
#include "MainWindow.h"
#include "ColorWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    ColorWidget w;
    w.show();
    return a.exec();
}
