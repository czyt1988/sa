#include "SARibbonMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SARibbonMainWindow w;
    w.show();

    return a.exec();
}
