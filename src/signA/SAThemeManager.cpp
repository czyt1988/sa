#include "SAThemeManager.h"
#include <QFile>
#include <QApplication>
#include <QDir>





SAThemeManager::SAThemeManager()
{

}

void SAThemeManager::setStyle(const QString &styleName)
{
    QFile file(QApplication::applicationDirPath()+QDir::separator()+"theme"+QDir::separator()+styleName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        SAThemeManager tmp;
        tmp.setDefault();
        return;
    }
    QString ss = file.readAll();
    qApp->setStyleSheet(ss);
}

bool SAThemeManager::setDefault()
{
    QFile file(":/default/theme/default.qss");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    qApp->setStyleSheet(file.readAll());
    return true;
}


