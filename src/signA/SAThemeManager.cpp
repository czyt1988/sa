#include "SAThemeManager.h"
#include <QFile>
#include <QApplication>
#include <QDir>





SAThemeManager::SAThemeManager()
{

}

bool SAThemeManager::getStyleString(const QString &styleName, QString &mainStyle, QString &ribbonStyle)
{
    QString themeFolder = QApplication::applicationDirPath()+QDir::separator()+"theme"+QDir::separator()+styleName;
    QFile file(themeFolder+QDir::separator()+"mian.qss" );
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    mainStyle = file.readAll();
    file.close();

    file.setFileName(themeFolder+QDir::separator()+"ribbon.qss" );
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    ribbonStyle = file.readAll();
    file.close();
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


