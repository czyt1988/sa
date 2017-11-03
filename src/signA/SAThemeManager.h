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
    static QString getStyleString(const QString &styleName);
    static QString getDefaultStyleString();
private:
};

#endif // SATHEMEMANAGER_H
