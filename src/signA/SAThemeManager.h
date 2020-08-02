#ifndef SATHEMEMANAGER_H
#define SATHEMEMANAGER_H
#include <QMainWindow>
///
/// \brief 用于管理主题
///
class SAThemeManager
{
public:
    SAThemeManager();
    static bool setStyle(const QString& styleName, QWidget *mainWindow);
    static QString getThemeDir();
    static QStringList getSkinList();
    static QString currentStyleName();

private:
    static QString getDefaultStyleString();
    static bool getStyleString(const QString& styleName, QString& mainStyle, QString& ribbonStyle);

    static QString s_currentStyleName;
};

#endif // SATHEMEMANAGER_H
