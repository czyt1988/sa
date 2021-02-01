#include "SAThemeManager.h"
#include <QFile>
#include <QApplication>
#include <QDir>



QString SAThemeManager::s_currentStyleName = "normal";

SAThemeManager::SAThemeManager()
{
}


bool SAThemeManager::setStyle(const QString& styleName, QWidget *mainWindow)
{
    QString mainStyle;
    QString ribbonStyle;

    if (!SAThemeManager::getStyleString(styleName, mainStyle, ribbonStyle)) {
        return (false);
    }
    qApp->setStyleSheet(mainStyle);
    mainWindow->setStyleSheet(ribbonStyle);
    s_currentStyleName = styleName;
    mainWindow->repaint();
    return (true);
}


bool SAThemeManager::getStyleString(const QString& styleName, QString& mainStyle, QString& ribbonStyle)
{
    QString themeFolder = getThemeDir()+QDir::separator()+styleName;
    QFile file(themeFolder+QDir::separator()+"main.qss");

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return (false);
    }
    mainStyle = file.readAll();
    file.close();

    file.setFileName(themeFolder+QDir::separator()+"ribbon.qss");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return (false);
    }
    ribbonStyle = file.readAll();
    file.close();
    return (true);
}


QString SAThemeManager::getDefaultStyleString()
{
    QFile file(":/default/theme/default.qss");

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return ("");
    }
    return (file.readAll());
}


QString SAThemeManager::getThemeDir()
{
    return (QApplication::applicationDirPath()+QDir::separator()+"theme");
}


QStringList SAThemeManager::getSkinList()
{
    QDir dir(SAThemeManager::getThemeDir());
    QStringList skinList = dir.entryList(QDir::NoDotAndDotDot|QDir::Dirs);

    return (skinList);
}


QString SAThemeManager::currentStyleName()
{
    return (s_currentStyleName);
}
