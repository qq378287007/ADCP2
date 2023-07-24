#include <QApplication>
#include "MainWindow.h"
#include "ColorWidget.h"
#include "InfoWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //ColorWidget w;
    //InfoWidget w;
    w.show();
    return a.exec();
}
