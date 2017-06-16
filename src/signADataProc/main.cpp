
#include <QApplication>
#include <QTextCodec>
#include <QStringList>
#include <QScopedPointer>
#include "SADataProcClient.h"
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList argsList = a.arguments();
    QMessageBox::information(nullptr,"pro",argsList.join(','));
    //调用必须后面跟随调用者的pid
    if(argsList.size()<2)
    {
        return 1;
    }
    bool isSuccess = false;
    uint pid = argsList[1].toUInt(&isSuccess);
    if(!isSuccess)
    {
        return 2;
    }
    SADataProcClient client;
    client.setPid(pid);
    client.shakeHand();
    return a.exec();
}
