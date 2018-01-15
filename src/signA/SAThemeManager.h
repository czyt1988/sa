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
    static bool getStyleString(const QString &styleName,QString& mainStyle,QString& ribbonStyle);
    static QString getDefaultStyleString();
private:
};

#endif // SATHEMEMANAGER_H
