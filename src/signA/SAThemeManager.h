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
    static void setStyle(const QString &styleName);
    bool setDefault();
private:
};

#endif // SATHEMEMANAGER_H
