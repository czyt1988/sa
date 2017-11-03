#include "SAThemeManager.h"
#include <QFile>
#include <QApplication>
#include <QDir>





SAThemeManager::SAThemeManager()
{

}

QString SAThemeManager::getStyleString(const QString &styleName)
{
    QFile file(QApplication::applicationDirPath()+QDir::separator()+"theme"+QDir::separator()+styleName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return SAThemeManager::getDefaultStyleString();
    }
    return file.readAll();
}

QString SAThemeManager::getDefaultStyleString()
{
    QFile file(":/default/theme/default.qss");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return "";
    }
    return file.readAll();
}


